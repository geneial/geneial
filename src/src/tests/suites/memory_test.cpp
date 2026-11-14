#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE memory
#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif

#include <atomic>
#include <functional>
#include <geneial/algorithm/SteadyStateAlgorithm.h>
#include <geneial/algorithm/diagnostics/Diagnostics.h>
#include <geneial/algorithm/observer/BestChromosomeObserver.h>
#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/core/population/management/ResourcePool.h>
#include <geneial/utility/ThreadedExecutionManager.h>
#include <limits>
#include <sstream>
#include <thread>

using namespace geneial::population::management;
namespace
{
struct Resource
{
    static inline std::atomic<int> live = 0;
    std::function<void()> onDestruction;
    Resource()
    {
        ++live;
    }
    ~Resource()
    {
        if (onDestruction)
            onDestruction();
        --live;
    }
};
struct Payload : EventData
{
    static inline int live = 0;
    Payload()
    {
        ++live;
    }
    ~Payload() override
    {
        --live;
    }
};
using Factory = geneial::population::chromosome::MultiValueChromosomeFactory<int, double>;
auto evaluator(double value)
{
    return std::make_shared<geneial::FunctionFitnessEvaluator<int>>([value](std::span<const int>) { return value; });
}
}

BOOST_AUTO_TEST_CASE(resource_pool_is_bounded_and_can_be_disabled_or_pruned)
{
    BOOST_REQUIRE_EQUAL(Resource::live, 0);
    ResourcePool<Resource> pool(2);
    std::vector<Resource *> resources;
    for (unsigned int index = 0; index < 20; ++index)
        resources.push_back(new Resource);
    for (auto *resource : resources)
        pool.free(std::move(resource));
    BOOST_CHECK_EQUAL(pool.size(), 2);
    BOOST_CHECK_EQUAL(Resource::live, 2);
    pool.setCapacity(1);
    BOOST_CHECK_EQUAL(Resource::live, 1);
    Resource *resource = nullptr;
    BOOST_REQUIRE(pool.retrieve(resource));
    resource->onDestruction = [&pool] { BOOST_CHECK_EQUAL(pool.size(), 0); };
    pool.setCapacity(0);
    pool.free(std::move(resource));
    BOOST_CHECK(!resource);
    BOOST_CHECK_EQUAL(Resource::live, 0);
    pool.setCapacity(4);
    resource = new Resource;
    pool.free(std::move(resource));
    pool.clear();
    BOOST_CHECK_EQUAL(pool.size(), 0);
    BOOST_CHECK_EQUAL(Resource::live, 0);
}

BOOST_AUTO_TEST_CASE(resource_pool_handles_concurrent_returns_without_losing_ownership)
{
    {
        ResourcePool<Resource> pool(8);
        std::vector<std::jthread> workers;
        for (unsigned int worker = 0; worker < 4; ++worker)
            workers.emplace_back(
                [&pool]
                {
                    for (unsigned int iteration = 0; iteration < 2000; ++iteration)
                    {
                        Resource *resource = nullptr;
                        if (!pool.retrieve(resource))
                            resource = new Resource;
                        pool.free(std::move(resource));
                    }
                });
        workers.clear();
        BOOST_CHECK_LE(pool.size(), 8);
    }
    BOOST_CHECK_EQUAL(Resource::live, 0);
}

BOOST_AUTO_TEST_CASE(factory_recycling_resets_age_and_does_not_retain_old_evaluators)
{
    auto oldEvaluator = evaluator(1);
    std::weak_ptr<geneial::FitnessEvaluator<double>> observed = oldEvaluator;
    Factory::Builder builder(oldEvaluator);
    auto factory = std::dynamic_pointer_cast<Factory>(builder.create());
    factory->setResourcePoolCapacity(1);
    {
        auto chromosome = factory->createChromosome();
        chromosome->setAge(123);
        BOOST_CHECK_EQUAL(chromosome->getFitness().get(), 1);
    }
    BOOST_CHECK_EQUAL(factory->getCachedChromosomeCount(), 1);
    builder.getSettings().setFitnessEvaluator(evaluator(2));
    oldEvaluator.reset();
    BOOST_CHECK(observed.expired());
    auto recycled = factory->createChromosome();
    BOOST_CHECK_EQUAL(recycled->getAge(), 0);
    BOOST_CHECK(!recycled->hasFitness());
    BOOST_CHECK_EQUAL(recycled->getFitness().get(), 2);
    recycled.reset();
    factory->clearResourcePool();
    BOOST_CHECK_EQUAL(factory->getCachedChromosomeCount(), 0);
}

BOOST_AUTO_TEST_CASE(outstanding_chromosomes_do_not_keep_their_factory_alive)
{
    auto objective = evaluator(3);
    auto factory = Factory::Builder(objective).create();
    std::weak_ptr<geneial::population::chromosome::BaseChromosomeFactory<double>> weakFactory = factory;
    auto chromosome = factory->createChromosome();
    factory.reset();
    BOOST_CHECK(weakFactory.expired());
    BOOST_CHECK_EQUAL(chromosome->getFitness().get(), 3);
    chromosome.reset();
}

BOOST_AUTO_TEST_CASE(diagnostic_events_have_explicit_bounded_retention)
{
    StatisticBookkeeper keeper(2);
    for (unsigned int value = 0; value < 10; ++value)
        EventValueData<unsigned int>::create(keeper, "event", value);
    BOOST_CHECK_EQUAL(keeper.getEvents().size(), 2);
    BOOST_CHECK_EQUAL(keeper.getDroppedEventCount(), 8);
    keeper.setCapacity(1);
    BOOST_CHECK_EQUAL(keeper.getEvents().size(), 1);
    BOOST_CHECK_EQUAL(keeper.getDroppedEventCount(), 9);
    keeper.clear();
    BOOST_CHECK(keeper.getEvents().empty());
    BOOST_CHECK_EQUAL(keeper.getDroppedEventCount(), 0);
    keeper.setCapacity(0);
    BOOST_CHECK(!keeper.scopedEventTrace("timer"));
    keeper.traceEvent("event", new Payload);
    BOOST_CHECK_EQUAL(Payload::live, 0);
    BOOST_CHECK_EQUAL(keeper.getDroppedEventCount(), 2);
}

BOOST_AUTO_TEST_CASE(diagnostic_payloads_are_owned_before_name_validation)
{
    StatisticBookkeeper keeper;
    BOOST_CHECK_THROW(keeper.traceEvent(nullptr, new Payload), std::invalid_argument);
    BOOST_CHECK_EQUAL(Payload::live, 0);
    BOOST_CHECK_THROW(keeper.scopedEventTrace(nullptr), std::invalid_argument);
    DefaultBookkeeper disabled;
    disabled.traceEvent("ignored", new Payload);
    BOOST_CHECK_EQUAL(Payload::live, 0);
}

BOOST_AUTO_TEST_CASE(scoped_timers_may_outlive_their_bookkeeper)
{
    std::unique_ptr<ScopedTrace> timer;
    {
        StatisticBookkeeper keeper;
        timer = keeper.scopedEventTrace("timer");
    }
    BOOST_CHECK_NO_THROW(timer.reset());
    StatisticBookkeeper keeper(1);
    {
        auto active = keeper.scopedEventTrace("timer");
    }
    BOOST_REQUIRE_EQUAL(keeper.getEvents().size(), 1);
    const auto *duration = dynamic_cast<const EventValueData<double> *>(keeper.getEvents().begin()->second.get());
    BOOST_REQUIRE(duration);
    BOOST_CHECK_GE(duration->getValue(), 0);
}

BOOST_AUTO_TEST_CASE(timing_diagnostics_ignore_missing_or_incompatible_payloads)
{
    StatisticBookkeeper keeper;
    keeper.traceEvent("timer");
    keeper.traceEvent("timer", nullptr);
    EventValueData<int>::create(keeper, "timer", 4);
    EventValueData<double>::create(keeper, "timer", std::numeric_limits<double>::quiet_NaN());
    EventValueData<double>::create(keeper, "timer", 2);
    std::ostringstream stream;
    geneial::algorithm::printTiming("timer", keeper.getEvents(), stream);
    BOOST_CHECK_NE(stream.str().find("2.000000"), std::string::npos);
    BOOST_CHECK_THROW(geneial::algorithm::printTiming(nullptr, keeper.getEvents(), stream), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(waiting_for_workers_also_releases_completed_task_captures)
{
    geneial::utility::ThreadedExecutionManager workers(2);
    workers.setAmountPerThread(8);
    auto resource = std::make_shared<Resource>();
    std::weak_ptr<Resource> observed = resource;
    workers.addTask([resource] {});
    resource.reset();
    workers.waitForTasks();
    BOOST_CHECK(observed.expired());
    BOOST_CHECK_EQUAL(Resource::live, 0);
}

BOOST_AUTO_TEST_CASE(diagnostic_facade_validates_lifecycles_and_reports_retained_events)
{
    using Diagnostics = geneial::algorithm::Diagnostics<double>;
    BOOST_CHECK_THROW(Diagnostics(nullptr), std::invalid_argument);
    geneial::algorithm::SteadyStateAlgorithm<double>::Builder builder;
    builder.setChromosomeFactory(Factory::Builder(evaluator(1)).create());
    builder.setStoppingCriterion(
        std::make_shared<geneial::algorithm::stopping_criteria::MaxGenerationCriterion<double>>(0));
    auto algorithm = builder.create();
    Diagnostics diagnostics(algorithm);
    std::ostringstream report;
    for (const char *event : {"COUPLING_TEST", "CROSSOVER_TEST", "MUTATION_TEST", "REPLACEMENT_TEST", "SELECTION_TEST"})
    {
        algorithm->getBookkeeper()->traceEvent(event);
        algorithm->getBookkeeper()->traceEvent(event);
    }
    diagnostics.analyseAll(report);
    for (const char *event : {"COUPLING_TEST", "CROSSOVER_TEST", "MUTATION_TEST", "REPLACEMENT_TEST", "SELECTION_TEST"})
        BOOST_CHECK_NE(report.str().find(std::string(event) + " x 2"), std::string::npos);
    BOOST_CHECK_NE(report.str().find("not a convergence history"), std::string::npos);
    BOOST_CHECK_NE(report.str().find("size 0"), std::string::npos);
    algorithm->setBookkeeper(std::make_shared<DefaultBookkeeper>());
    BOOST_CHECK_THROW(diagnostics.analyseTimeSpent(report), std::logic_error);
    algorithm->solve();
    BOOST_CHECK_THROW((Diagnostics(algorithm)), std::logic_error);
    diagnostics.anaylseConvergence(report);
    BOOST_CHECK_NE(report.str().find("worst"), std::string::npos);
}

BOOST_AUTO_TEST_CASE(best_observer_initializes_its_baseline_before_comparison)
{
    struct Observer : geneial::algorithm::BestChromosomeObserver<double>
    {
        std::vector<double> improvements;
        void updateNewBestChromosome(BaseManager<double> &manager) override
        {
            improvements.push_back(manager.getHighestFitness());
        }
    } observer;
    Factory::Builder builder(evaluator(-3));
    auto factory = builder.create();
    auto manager = BaseManager<double>::create(factory);
    for (double score : {-3.0, -3.0, -2.0, -4.0})
    {
        builder.getSettings().setFitnessEvaluator(evaluator(score));
        manager->getPopulation().clearChromosomes();
        manager->getPopulation().insertChromosome(factory->createChromosome());
        observer.updateAfterGeneration(*manager);
    }
    BOOST_REQUIRE_EQUAL(observer.improvements.size(), 2);
    BOOST_CHECK_EQUAL(observer.improvements.front(), -3);
    BOOST_CHECK_EQUAL(observer.improvements.back(), -2);
}
