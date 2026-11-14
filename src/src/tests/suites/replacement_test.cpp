#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE replacement
#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif

#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/operations/replacement/ReplaceRandomOperation.h>
#include <geneial/core/operations/replacement/ReplaceWorstOperation.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>

#include <limits>

namespace
{
using Chromosome = geneial::population::chromosome::MultiValueChromosome<int, double>;
using Factory = geneial::population::chromosome::MultiValueChromosomeFactory<int, double>;
using Manager = geneial::population::management::BaseManager<double>;
using Population = geneial::population::Population<double>;
using namespace geneial::operation::replacement;

struct Fixture
{
    geneial::FitnessEvaluator<double>::ptr evaluator =
        std::make_shared<geneial::FunctionFitnessEvaluator<int>>([](std::span<const int> values) { return values[0]; });
    std::shared_ptr<Manager> manager = Manager::create(Factory::Builder(evaluator).create());

    Fixture()
    {
        geneial::utility::Random::initialize(42);
        manager->getPopulationSettings().setMaxChromosomes(5);
        for (int value = 0; value < 5; ++value)
            manager->getPopulation().insertChromosome(chromosome(value));
    }

    Chromosome::ptr chromosome(int value)
    {
        auto result = std::make_shared<Chromosome>(evaluator);
        result->setValueContainer({value});
        return result;
    }

    template <typename Operation>
    void replace(BaseReplacementSettings settings, Population::chromosome_container children)
    {
        auto operation = typename Operation::Builder().create();
        operation->setSettings(settings);
        operation->doReplace(manager->getPopulation(), {}, children, *manager);
    }
};
} // namespace

BOOST_FIXTURE_TEST_SUITE(Replacements, Fixture)

BOOST_AUTO_TEST_CASE(random_fixed_budget_does_not_insert_every_offspring)
{
    replace<ReplaceRandomOperation<double>>(
        BaseReplacementSettings{BaseReplacementSettings::REPLACE_FIXED_NUMBER, 1, 1},
        {chromosome(10), chromosome(11), chromosome(12), chromosome(13)});
    BOOST_CHECK_EQUAL(manager->getPopulation().getSize(), 5);
    BOOST_CHECK_EQUAL(manager->getHighestFitness(), 10);
    BOOST_CHECK(manager->getPopulation().containsChromosome(chromosome(4)));
}

BOOST_AUTO_TEST_CASE(random_replacement_with_no_children_does_not_remove_members)
{
    replace<ReplaceRandomOperation<double>>(
        BaseReplacementSettings{BaseReplacementSettings::REPLACE_FIXED_NUMBER, 1, 1}, {});
    BOOST_CHECK_EQUAL(manager->getPopulation().getSize(), 5);
    BOOST_CHECK_EQUAL(manager->getHighestFitness(), 4);
}

BOOST_AUTO_TEST_CASE(random_replacement_clamps_to_nonelite_members)
{
    replace<ReplaceRandomOperation<double>>(
        BaseReplacementSettings{BaseReplacementSettings::REPLACE_ALL_OFFSPRING, 0, 2},
        {chromosome(10), chromosome(11), chromosome(12), chromosome(13)});
    BOOST_CHECK_EQUAL(manager->getPopulation().getSize(), 5);
    BOOST_CHECK(manager->getPopulation().containsChromosome(chromosome(3)));
    BOOST_CHECK(manager->getPopulation().containsChromosome(chromosome(4)));
    BOOST_CHECK_EQUAL(manager->getHighestFitness(), 12);
}

BOOST_AUTO_TEST_CASE(oversized_elitism_and_zero_budgets_are_noops)
{
    for (const auto settings : {BaseReplacementSettings{BaseReplacementSettings::REPLACE_ALL_OFFSPRING, 0, 20},
                                BaseReplacementSettings{BaseReplacementSettings::REPLACE_FIXED_NUMBER, 0, 0}})
    {
        replace<ReplaceRandomOperation<double>>(settings, {chromosome(10)});
        replace<ReplaceWorstOperation<double>>(settings, {chromosome(10)});
        BOOST_CHECK_EQUAL(manager->getPopulation().getSize(), 5);
        BOOST_CHECK_EQUAL(manager->getHighestFitness(), 4);
    }
}

BOOST_AUTO_TEST_CASE(random_duplicate_children_neither_shrink_population_nor_consume_unique_slots)
{
    replace<ReplaceRandomOperation<double>>(
        BaseReplacementSettings{BaseReplacementSettings::REPLACE_FIXED_NUMBER, 2, 1},
        {chromosome(4), chromosome(10), chromosome(10), chromosome(11)});
    BOOST_CHECK_EQUAL(manager->getPopulation().getSize(), 5);
    BOOST_CHECK(manager->getPopulation().containsChromosome(chromosome(10)));
    BOOST_CHECK(manager->getPopulation().containsChromosome(chromosome(11)));
    BOOST_CHECK(manager->getPopulation().containsChromosome(chromosome(4)));
}

BOOST_AUTO_TEST_CASE(worst_duplicate_children_do_not_hide_later_unique_candidates)
{
    replace<ReplaceWorstOperation<double>>(BaseReplacementSettings{BaseReplacementSettings::REPLACE_FIXED_NUMBER, 2, 1},
                                           {chromosome(10), chromosome(10), chromosome(11)});
    BOOST_CHECK_EQUAL(manager->getPopulation().getSize(), 5);
    BOOST_CHECK(manager->getPopulation().containsChromosome(chromosome(10)));
    BOOST_CHECK(manager->getPopulation().containsChromosome(chromosome(11)));
    BOOST_CHECK(manager->getPopulation().containsChromosome(chromosome(4)));
}

BOOST_AUTO_TEST_CASE(invalid_fitness_does_not_delete_existing_members)
{
    auto invalid = chromosome(10);
    invalid->setFitness(std::make_unique<geneial::Fitness<double>>(std::numeric_limits<double>::quiet_NaN()));
    BOOST_CHECK_THROW(replace<ReplaceRandomOperation<double>>(
                          BaseReplacementSettings{BaseReplacementSettings::REPLACE_ALL_OFFSPRING, 0, 1}, {invalid}),
                      std::invalid_argument);
    BOOST_CHECK_EQUAL(manager->getPopulation().getSize(), 5);
    BOOST_CHECK_EQUAL(manager->getPopulation().getHashMap().size(), 5);
}

BOOST_AUTO_TEST_CASE(empty_populations_and_one_member_elites_are_safe)
{
    auto &population = manager->getPopulation();
    population.clearChromosomes();
    replace<ReplaceRandomOperation<double>>(
        BaseReplacementSettings{BaseReplacementSettings::REPLACE_ALL_OFFSPRING, 0, 1}, {chromosome(10)});
    BOOST_CHECK_EQUAL(population.getSize(), 0);
    population.insertChromosome(chromosome(1));
    replace<ReplaceRandomOperation<double>>(
        BaseReplacementSettings{BaseReplacementSettings::REPLACE_ALL_OFFSPRING, 0, 1}, {chromosome(10)});
    BOOST_CHECK_EQUAL(population.getSize(), 1);
    BOOST_CHECK_EQUAL(manager->getHighestFitness(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
