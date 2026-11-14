#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE population
#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif

#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/operations/replacement/ReplaceWorstOperation.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/utility/ThreadedExecutionManager.h>

#include <atomic>
#include <limits>
#include <type_traits>

namespace
{
using Chromosome = geneial::population::chromosome::MultiValueChromosome<int, double>;
using Factory = geneial::population::chromosome::MultiValueChromosomeFactory<int, double>;
using Manager = geneial::population::management::BaseManager<double>;
using Population = geneial::population::Population<double>;
static_assert(!std::is_copy_constructible_v<Population>);
static_assert(!std::is_copy_assignable_v<Population>);
static_assert(!std::is_move_constructible_v<Population>);
static_assert(!std::is_move_assignable_v<Population>);

class CollisionChromosome final : public Chromosome
{
  public:
    CollisionChromosome(const geneial::FitnessEvaluator<double>::ptr &evaluator, int value) : Chromosome(evaluator)
    {
        setValueContainer({value});
    }

    chromsome_hash getHash() const override
    {
        return 7;
    }
};

struct Fixture
{
    geneial::FitnessEvaluator<double>::ptr evaluator =
        std::make_shared<geneial::FunctionFitnessEvaluator<int>>([](std::span<const int> values) { return values[0]; });
    std::shared_ptr<Manager> manager = Manager::create(Factory::Builder(evaluator).create());

    Chromosome::ptr chromosome(int value)
    {
        return std::make_shared<CollisionChromosome>(evaluator, value);
    }
};
} // namespace

BOOST_FIXTURE_TEST_SUITE(Populations, Fixture)

BOOST_AUTO_TEST_CASE(hash_collisions_preserve_distinct_genomes_but_reject_equal_values)
{
    auto &population = manager->getPopulation();
    BOOST_CHECK(population.insertChromosome(chromosome(1)));
    BOOST_CHECK(population.insertChromosome(chromosome(2)));
    BOOST_CHECK(!population.insertChromosome(chromosome(1)));
    BOOST_CHECK_EQUAL(population.getSize(), 2);
    BOOST_CHECK_EQUAL(population.getHashMap().size(), 2);
    BOOST_CHECK_EQUAL(population.getFitnessMap().size(), 2);
}

BOOST_AUTO_TEST_CASE(batch_deduplication_checks_values_in_each_collision_bucket)
{
    auto &population = manager->getPopulation();
    population.insertChromosome(chromosome(1));
    Population::chromosome_container batch{chromosome(1), chromosome(2), chromosome(3), chromosome(2)};
    BOOST_CHECK_EQUAL(population.insertChromosomeContainer(batch), 2);
    BOOST_CHECK_EQUAL(batch.size(), 2);
    BOOST_CHECK_EQUAL(population.getSize(), 3);
    Population::chromosome_container candidates{chromosome(1), chromosome(4), chromosome(5), chromosome(4)};
    BOOST_CHECK_EQUAL(population.removeDuplicates(candidates), 2);
    BOOST_REQUIRE_EQUAL(candidates.size(), 2);
    BOOST_CHECK_EQUAL(candidates[0]->getFitness().get(), 4);
    BOOST_CHECK_EQUAL(candidates[1]->getFitness().get(), 5);
    manager->getPopulationSettings().setMaxChromosomes(3);
    auto replacement = geneial::operation::replacement::ReplaceWorstOperation<double>::Builder().create();
    replacement->doReplace(population, {}, candidates, *manager);
    BOOST_CHECK_EQUAL(population.getSize(), 3);
    BOOST_CHECK(population.containsChromosome(chromosome(4)));
    BOOST_CHECK(population.containsChromosome(chromosome(5)));
    using FloatingChromosome = geneial::population::chromosome::MultiValueChromosome<double, double>;
    const auto floatingEvaluator =
        std::make_shared<geneial::FunctionFitnessEvaluator<double>>([](std::span<const double>) { return 1.0; });
    auto nonreflexive = std::make_shared<FloatingChromosome>(floatingEvaluator);
    nonreflexive->setValueContainer({std::numeric_limits<double>::quiet_NaN()});
    Population::chromosome_container repeated{nonreflexive, nonreflexive};
    BOOST_CHECK_EQUAL(population.insertChromosomeContainer(repeated), 1);
    BOOST_CHECK(population.containsChromosome(nonreflexive));
    BOOST_CHECK(!population.insertChromosome(nonreflexive));
    population.doAge();
    BOOST_CHECK_EQUAL(nonreflexive->getAge(), 1);
}

BOOST_AUTO_TEST_CASE(hash_lookup_and_removal_keep_both_indexes_in_sync)
{
    auto &population = manager->getPopulation();
    const auto first = chromosome(1);
    const auto second = chromosome(2);
    second->setFitness(std::make_unique<geneial::Fitness<double>>(1));
    population.insertChromosome(first);
    population.insertChromosome(second);
    const auto found = population.getChromosomeByHash(7);
    BOOST_CHECK(found == first || found == second);
    BOOST_CHECK(!population.getChromosomeByHash(8));
    population.removeChromosome(first);
    BOOST_CHECK(population.getChromosomeByHash(7) == second);
    BOOST_CHECK_EQUAL(population.getHashMap().size(), 1);
    population.removeChromosome(second);
    BOOST_CHECK(!population.hashExists(7));
    BOOST_CHECK_EQUAL(population.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(removing_a_nonmember_is_rejected_without_erasing_a_collision)
{
    auto &population = manager->getPopulation();
    population.insertChromosome(chromosome(1));
    BOOST_CHECK_THROW(population.removeChromosome(chromosome(1)), std::invalid_argument);
    BOOST_CHECK_THROW(population.removeChromosome(chromosome(2)), std::invalid_argument);
    BOOST_CHECK_THROW(population.removeChromosome(nullptr), std::invalid_argument);
    BOOST_CHECK_EQUAL(population.getSize(), 1);
    BOOST_CHECK_EQUAL(population.getHashMap().size(), 1);
}

BOOST_AUTO_TEST_CASE(aging_visits_every_member_in_a_collision_bucket_once)
{
    auto &population = manager->getPopulation();
    BOOST_CHECK(!population.getOldestChromosome());
    BOOST_CHECK(!population.getYoungestChromosome());
    const auto first = chromosome(1);
    const auto second = chromosome(2);
    first->setAge(10);
    second->setAge(3);
    population.insertChromosome(first);
    population.insertChromosome(second);
    population.doAge();
    BOOST_CHECK_EQUAL(population.getAge(), 1);
    BOOST_CHECK_EQUAL(first->getAge(), 11);
    BOOST_CHECK_EQUAL(second->getAge(), 4);
    BOOST_CHECK(population.getOldestChromosome() == first);
    BOOST_CHECK(population.getYoungestChromosome() == second);
}

BOOST_AUTO_TEST_CASE(null_and_nonfinite_candidates_do_not_enter_either_index)
{
    auto &population = manager->getPopulation();
    BOOST_CHECK_THROW(population.insertChromosome(nullptr), std::invalid_argument);
    for (const auto score : {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::infinity(),
                             -std::numeric_limits<double>::infinity()})
    {
        auto candidate = chromosome(1);
        candidate->setFitness(std::make_unique<geneial::Fitness<double>>(score));
        BOOST_CHECK_THROW(population.insertChromosome(candidate), std::invalid_argument);
        BOOST_CHECK_EQUAL(population.getSize(), 0);
        BOOST_CHECK(population.getHashMap().empty());
    }
}

BOOST_AUTO_TEST_CASE(batch_fitness_failures_leave_indexes_unchanged_and_executor_reusable)
{
    std::atomic<unsigned int> evaluations = 0;
    evaluator = std::make_shared<geneial::FunctionFitnessEvaluator<int>>(
        [&](std::span<const int> values)
        {
            ++evaluations;
            if (values[0] == 2)
                throw std::runtime_error("deliberate fitness failure");
            return values[0];
        });
    manager->setExecutionManager(std::make_unique<geneial::utility::ThreadedExecutionManager>(3));
    auto &population = manager->getPopulation();
    Population::chromosome_container batch{chromosome(1), chromosome(2), chromosome(3), chromosome(1)};
    BOOST_CHECK_THROW(population.insertChromosomeContainer(batch), std::runtime_error);
    BOOST_CHECK_EQUAL(evaluations.load(), 3);
    BOOST_CHECK(population.getFitnessMap().empty());
    BOOST_CHECK(population.getHashMap().empty());
    Population::chromosome_container valid{chromosome(4), chromosome(5)};
    BOOST_CHECK_EQUAL(population.insertChromosomeContainer(valid), 2);
    BOOST_CHECK_EQUAL(population.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(replacing_the_whole_population_is_transactional_for_invalid_candidates)
{
    auto &population = manager->getPopulation();
    population.insertChromosome(chromosome(1));
    population.setAge(12);
    Population::chromosome_container valid{chromosome(2), chromosome(2), chromosome(3)};
    population.replacePopulation(valid);
    BOOST_CHECK_EQUAL(population.getSize(), 2);
    BOOST_CHECK_EQUAL(valid.size(), 2);
    BOOST_CHECK_EQUAL(population.getAge(), 12);
    auto invalid = chromosome(5);
    invalid->setFitness(std::make_unique<geneial::Fitness<double>>(std::numeric_limits<double>::infinity()));
    Population::chromosome_container rejected{chromosome(4), invalid};
    BOOST_CHECK_THROW(population.replacePopulation(rejected), std::invalid_argument);
    BOOST_CHECK_EQUAL(population.getSize(), 2);
    BOOST_CHECK(population.containsChromosome(chromosome(2)));
    BOOST_CHECK(population.containsChromosome(chromosome(3)));
    BOOST_CHECK_EQUAL(rejected.size(), 2);
    BOOST_CHECK_EQUAL(population.getHashMap().size(), 2);
}

BOOST_AUTO_TEST_CASE(null_manager_dependencies_are_rejected_without_replacing_valid_ones)
{
    const auto factory = &manager->getChromosomeFactory();
    const auto executor = &manager->getExecutionManager();
    const auto bookkeeper = manager->getBookkeeper();
    BOOST_CHECK_THROW(manager->setChromosomeFactory(nullptr), std::invalid_argument);
    BOOST_CHECK_THROW(manager->setExecutionManager(nullptr), std::invalid_argument);
    BOOST_CHECK_THROW(manager->setBookkeeper(nullptr), std::invalid_argument);
    BOOST_CHECK(&manager->getChromosomeFactory() == factory);
    BOOST_CHECK(&manager->getExecutionManager() == executor);
    BOOST_CHECK(manager->getBookkeeper() == bookkeeper);
}

BOOST_AUTO_TEST_SUITE_END()
