#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE regression
#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif

#include <geneial/algorithm/SteadyStateAlgorithm.h>
#include <geneial/core/operations/crossover/MultiValuePermutationCrossover.h>
#include <geneial/core/operations/selection/FitnessProportionalSelection.h>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>
#include <geneial/core/population/builder/PermutationChromosomeFactory.h>
#include <geneial/utility/Interpolators.h>
#include <geneial/utility/Random.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <set>
#include <thread>
#include <vector>

using namespace geneial;
using namespace geneial::population::chromosome;
using namespace geneial::operation::selection;
using geneial::population::management::BaseManager;
using geneial::utility::Random;

class FirstValueEvaluator : public FitnessEvaluator<double>
{
  public:
    std::unique_ptr<Fitness<double>> evaluate(const BaseChromosome<double> &chromosome) const override
    {
        const auto &values = dynamic_cast<const MultiValueChromosome<int, double> &>(chromosome).getContainer();
        return std::unique_ptr<Fitness<double>>(new Fitness<double>(values.front()));
    }
};

struct SeedFixture
{
    SeedFixture()
    {
        Random::initialize(42);
    }
};

BOOST_FIXTURE_TEST_SUITE(Regressions, SeedFixture)

BOOST_AUTO_TEST_CASE(random_is_reproducible_and_thread_local)
{
    const int first = Random::generate<int>();
    const double second = Random::generate<double>();
    Random::initialize(42);
    BOOST_CHECK_EQUAL(Random::generate<int>(), first);
    std::thread worker([] { Random::initialize(99); });
    worker.join();
    BOOST_CHECK_EQUAL(Random::generate<double>(), second);
}

BOOST_AUTO_TEST_CASE(negative_integer_sampling_is_uniform)
{
    std::vector<unsigned int> counts(10, 0);
    for (unsigned int trial = 0; trial < 100000; ++trial)
    {
        ++counts[Random::generate(-5, 4) + 5];
    }
    for (auto count : counts)
    {
        BOOST_CHECK(count > 9000 && count < 11000);
    }
}

BOOST_AUTO_TEST_CASE(random_handles_extreme_and_invalid_ranges)
{
    for (unsigned int trial = 0; trial < 1000; ++trial)
    {
        const int value = Random::generate(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        BOOST_CHECK(value >= std::numeric_limits<int>::min());
        Random::generate(0u, std::numeric_limits<unsigned int>::max());
        const double real = Random::generate(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
        BOOST_CHECK(std::isfinite(real));
        const double unit = Random::generate<double>();
        BOOST_CHECK(unit >= 0 && unit < 1);
        BOOST_CHECK_EQUAL(Random::generate(7.0, 7.0), 7.0);
    }
    BOOST_CHECK_THROW(Random::generate(2, 1), std::invalid_argument);
    BOOST_CHECK_THROW(Random::generate(0.0, std::numeric_limits<double>::infinity()), std::invalid_argument);
    BOOST_CHECK_THROW(Random::decision(std::numeric_limits<double>::quiet_NaN()), std::invalid_argument);
    BOOST_CHECK_THROW(Random::decision(-0.1), std::invalid_argument);
    BOOST_CHECK_THROW(Random::decision(1.1), std::invalid_argument);
    BOOST_CHECK(!Random::decision(0));
    BOOST_CHECK(Random::decision(1));
}

BOOST_AUTO_TEST_CASE(factories_keep_settings_alive_after_builders_die)
{
    auto evaluator = std::make_shared<FirstValueEvaluator>();
    const auto makePlain = [&evaluator]
    {
        MultiValueChromosomeFactory<int, double>::Builder builder(evaluator);
        builder.getSettings().setNum(12);
        return builder.create();
    };
    const auto makeContinuous = [&evaluator]
    {
        ContinousMultiValueChromosomeFactory<int, double>::Builder builder(evaluator);
        builder.getSettings().setNum(12);
        builder.getSettings().setEps(3);
        return builder.create();
    };
    const auto makePermutation = [&evaluator]
    {
        PermutationChromosomeFactory<int, double>::Builder builder(evaluator);
        builder.getSettings().setNum(12);
        return builder.create();
    };
    for (const auto &factory : {makePlain(), makeContinuous(), makePermutation()})
    {
        const auto chromosome =
            std::dynamic_pointer_cast<MultiValueChromosome<int, double>>(factory->createChromosome());
        BOOST_REQUIRE(chromosome);
        BOOST_CHECK_EQUAL(chromosome->getSize(), 12);
        BOOST_CHECK(std::isfinite(chromosome->getFitness().get()));
    }
}

BOOST_AUTO_TEST_CASE(permutation_settings_and_crossover_preserve_the_gene_set)
{
    auto evaluator = std::make_shared<FirstValueEvaluator>();
    PermutationChromosomeFactory<int, double>::Builder builder(evaluator);
    builder.getSettings().setNum(12);
    builder.getSettings().setPermutationRoundsMax(30);
    builder.getSettings().setPermutationRoundsMin(10);
    BOOST_CHECK_EQUAL(builder.getSettings().getPermutationRoundsMin(), 10);
    BOOST_CHECK_EQUAL(builder.getSettings().getPermutationRoundsMax(), 30);
    auto factory = std::dynamic_pointer_cast<MultiValueChromosomeFactory<int, double>>(builder.create());
    const auto crossover =
        geneial::operation::crossover::MultiValuePermutationCrossover<int, double>::Builder(factory).create();
    for (unsigned int trial = 0; trial < 1000; ++trial)
    {
        const auto children = crossover->doCrossover(factory->createChromosome(), factory->createChromosome());
        BOOST_REQUIRE_EQUAL(children.size(), 1);
        auto values = std::dynamic_pointer_cast<MultiValueChromosome<int, double>>(children.front())->getContainer();
        std::sort(values.begin(), values.end());
        for (unsigned int index = 0; index < values.size(); ++index)
        {
            BOOST_CHECK_EQUAL(values[index], index);
        }
    }
}

BOOST_AUTO_TEST_CASE(selection_favors_better_negative_fitness_and_handles_all_zero)
{
    auto evaluator = std::make_shared<FirstValueEvaluator>();
    MultiValueChromosomeFactory<int, double>::Builder factoryBuilder(evaluator);
    auto factory = factoryBuilder.create();
    auto manager = BaseManager<double>::create(factory);
    for (int score : {-10, -5, -1})
    {
        auto chromosome = std::dynamic_pointer_cast<MultiValueChromosome<int, double>>(factory->createChromosome());
        chromosome->getContainer()[0] = score;
        BOOST_REQUIRE(manager->getPopulation().insertChromosome(chromosome));
    }
    RouletteWheel<double> wheel(manager->getPopulation());
    std::vector<unsigned int> counts(3, 0);
    for (unsigned int sample = 0; sample < 1700; ++sample)
    {
        const double score = wheel.spin((sample + 0.5) / 1700.0)->getFitness().get();
        ++counts[score == -10 ? 0 : score == -5 ? 1 : 2];
    }
    BOOST_CHECK_EQUAL(counts[0], 100);
    BOOST_CHECK_EQUAL(counts[1], 600);
    BOOST_CHECK_EQUAL(counts[2], 1000);
    BOOST_CHECK_EQUAL(wheel.spin(1)->getFitness().get(), -1);
    FitnessProportionalSelection<double>::Builder selectionBuilder;
    selectionBuilder.getSettings().setNumberOfParents(3);
    auto selection = selectionBuilder.create();
    const auto selected = selection->doSelect(manager->getPopulation(), *manager);
    BOOST_REQUIRE_EQUAL(selected.size(), 3);
    BOOST_CHECK_EQUAL(selected.front()->getFitness().get(), -1);
    BOOST_CHECK_EQUAL(std::set<BaseChromosome<double>::ptr>(selected.begin(), selected.end()).size(), 3);
    auto zeroManager = BaseManager<double>::create(factory);
    for (int index = 0; index < 3; ++index)
    {
        auto chromosome = std::dynamic_pointer_cast<MultiValueChromosome<int, double>>(factory->createChromosome());
        chromosome->getContainer()[0] = 0;
        chromosome->getContainer()[1] = index;
        BOOST_REQUIRE(zeroManager->getPopulation().insertChromosome(chromosome));
    }
    BOOST_CHECK_EQUAL(selection->doSelect(zeroManager->getPopulation(), *zeroManager).size(), 3);
    selectionBuilder.getSettings().setNumberOfParents(4);
    BOOST_CHECK_THROW(selection->doSelect(manager->getPopulation(), *manager), std::invalid_argument);
    auto emptyManager = BaseManager<double>::create(factory);
    BOOST_CHECK_THROW(RouletteWheel<double>(emptyManager->getPopulation()), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(invalid_algorithm_builder_throws_a_standard_exception)
{
    BOOST_CHECK_THROW(geneial::algorithm::SteadyStateAlgorithm<double>::Builder().create(), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(coordinate_interpolation_uses_the_requested_curve)
{
    using geneial::utility::Interpolators;
    BOOST_CHECK_CLOSE(Interpolators::linearInterpolate(10.0, 0.0, 20.0, 4.0, 1.0), 12.5, 0.0001);
    BOOST_CHECK_CLOSE(Interpolators::linearInterpolate(20.0, 4.0, 10.0, 0.0, 1.0), 12.5, 0.0001);
    const double expected = 10 + 10 * (1 - std::cos(std::acos(-1.0) * 0.25)) / 2;
    BOOST_CHECK_CLOSE(Interpolators::cosineInterpolate(10.0, 0.0, 20.0, 4.0, 1.0), expected, 0.0001);
    BOOST_CHECK_CLOSE(Interpolators::cosineInterpolate(20.0, 4.0, 10.0, 0.0, 1.0), expected, 0.0001);
}

BOOST_AUTO_TEST_CASE(roulette_handles_the_full_finite_fitness_range)
{
    auto evaluator = std::make_shared<FirstValueEvaluator>();
    auto factory = MultiValueChromosomeFactory<int, double>::Builder(evaluator).create();
    auto manager = BaseManager<double>::create(factory);
    for (const double fitness : {-std::numeric_limits<double>::max(), std::numeric_limits<double>::max()})
    {
        auto chromosome = factory->createChromosome();
        chromosome->setFitness(std::make_unique<Fitness<double>>(fitness));
        BOOST_REQUIRE(manager->getPopulation().insertChromosome(chromosome));
    }
    RouletteWheel<double> wheel(manager->getPopulation());
    BOOST_CHECK_EQUAL(wheel.spin(0)->getFitness().get(), -std::numeric_limits<double>::max());
    BOOST_CHECK_EQUAL(wheel.spin(0.5)->getFitness().get(), std::numeric_limits<double>::max());
}

BOOST_AUTO_TEST_SUITE_END()
