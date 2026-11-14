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
