#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE mutation
#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif

#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/operations/choosing/ChooseRandom.h>
#include <geneial/core/operations/mutation/SmoothPeakMutationOperation.h>
#include <geneial/core/operations/mutation/UniformMutationOperation.h>
#include <geneial/core/operations/mutation/ValueSwapMutationOperation.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/utility/Smoothing.h>

#include <limits>
#include <numeric>
#include <utility>

namespace
{
using Chromosome = geneial::population::chromosome::MultiValueChromosome<int, double>;
using Factory = geneial::population::chromosome::MultiValueChromosomeFactory<int, double>;
using ContinuousFactory = geneial::population::chromosome::ContinousMultiValueChromosomeFactory<int, double>;
using Manager = geneial::population::management::BaseManager<double>;
using geneial::utility::Smoothing;
using namespace geneial::operation::mutation;

struct Fixture
{
    geneial::FitnessEvaluator<double>::ptr evaluator = std::make_shared<geneial::FunctionFitnessEvaluator<int>>(
        [](std::span<const int> values) { return std::accumulate(values.begin(), values.end(), 0.0); });
    std::shared_ptr<Factory> factory;
    std::shared_ptr<Manager> manager;

    Fixture()
    {
        geneial::utility::Random::initialize(42);
        Factory::Builder builder(evaluator);
        builder.getSettings().setNum(8);
        builder.getSettings().setRandomMin(0);
        builder.getSettings().setRandomMax(10);
        factory = std::dynamic_pointer_cast<Factory>(builder.create());
        manager = Manager::create(factory);
    }

    Chromosome::ptr chromosome(std::vector<int> values = {0, 1, 2, 3, 4, 5, 6, 7})
    {
        auto result = std::make_shared<Chromosome>(evaluator);
        result->setValueContainer(std::move(values));
        return result;
    }
};
} // namespace

BOOST_FIXTURE_TEST_SUITE(Mutations, Fixture)

BOOST_AUTO_TEST_CASE(uniform_and_swap_mutations_preserve_sources_and_their_cached_fitness)
{
    const auto original = chromosome();
    original->setAge(12);
    const auto expected = std::as_const(*original).getContainer();
    original->getFitness();
    const auto hash = original->getHash();
    auto chooseAll = geneial::operation::choosing::ChooseRandom<double>::Builder(1).create();
    UniformMutationOperation<int, double>::Builder uniform(factory);
    ValueSwapMutationOperation<int, double>::Builder swaps(factory);
    for (auto *builder : {static_cast<MultiValueChromosomeMutationOperation<int, double>::Builder *>(&uniform),
                          static_cast<MultiValueChromosomeMutationOperation<int, double>::Builder *>(&swaps)})
    {
        builder->setChoosingOperation(chooseAll);
        builder->getSettings().setMinimumPointsToMutate(1);
        builder->getSettings().setMaximumPointsToMutate(1);
        const auto result = builder->create()->doMutate({original}, *manager);
        BOOST_REQUIRE_EQUAL(result.size(), 1);
        const auto mutated = std::dynamic_pointer_cast<Chromosome>(result.front());
        BOOST_REQUIRE(mutated);
        BOOST_CHECK(mutated != original);
        BOOST_CHECK_EQUAL(mutated->getAge(), 0);
        BOOST_CHECK(original->hasFitness());
        BOOST_CHECK(original->hasCache());
        BOOST_CHECK_EQUAL(original->getHash(), hash);
        BOOST_CHECK_EQUAL(original->getAge(), 12);
        BOOST_CHECK(std::as_const(*original).getContainer() == expected);
        BOOST_CHECK(std::all_of(std::as_const(*mutated).getContainer().begin(),
                                std::as_const(*mutated).getContainer().end(),
                                [](int value) { return value >= 0 && value <= 10; }));
    }
}

BOOST_AUTO_TEST_CASE(swap_mutation_preserves_the_gene_multiset)
{
    auto original = chromosome();
    ValueSwapMutationOperation<int, double>::Builder swaps(factory);
    swaps.setChoosingOperation(geneial::operation::choosing::ChooseRandom<double>::Builder(1).create());
    swaps.getSettings().setMinimumPointsToMutate(1);
    swaps.getSettings().setMaximumPointsToMutate(1);
    for (unsigned int iteration = 0; iteration < 100; ++iteration)
    {
        const auto result = swaps.create()->doMutate({original}, *manager);
        auto mutated = std::dynamic_pointer_cast<Chromosome>(result.front());
        auto values = std::as_const(*mutated).getContainer();
        BOOST_CHECK(values != std::as_const(*original).getContainer());
        std::sort(values.begin(), values.end());
        BOOST_CHECK(values == std::as_const(*original).getContainer());
    }
}

BOOST_AUTO_TEST_CASE(unchosen_and_zero_point_mutations_have_defined_behavior)
{
    auto original = chromosome();
    UniformMutationOperation<int, double>::Builder uniform(factory);
    uniform.setChoosingOperation(geneial::operation::choosing::ChooseRandom<double>::Builder(0).create());
    auto result = uniform.create()->doMutate({original}, *manager);
    BOOST_REQUIRE_EQUAL(result.size(), 1);
    BOOST_CHECK(result.front() == original);
    uniform.setChoosingOperation(geneial::operation::choosing::ChooseRandom<double>::Builder(1).create());
    uniform.getSettings().setMinimumPointsToMutate(0);
    uniform.getSettings().setMaximumPointsToMutate(0);
    result = uniform.create()->doMutate({original}, *manager);
    BOOST_CHECK(result.front() != original);
    BOOST_CHECK(result.front()->equals(*original));
    BOOST_CHECK(uniform.create()->doMutate({}, *manager).empty());
}

BOOST_AUTO_TEST_CASE(smoothing_uses_the_correct_start_anchor_and_neighbor_limits)
{
    auto unanchored = chromosome({8, 0, 10});
    Smoothing::restoreSmoothness<int, double>(unanchored, 1, 0, 10, false, 0);
    BOOST_CHECK(std::as_const(*unanchored).getContainer() == std::vector<int>({8, 7, 8}));
    auto anchored = chromosome({9, 0, 10});
    Smoothing::restoreSmoothness<int, double>(anchored, 1, 0, 10, true, 5);
    BOOST_CHECK(std::as_const(*anchored).getContainer() == std::vector<int>({6, 5, 6}));
    auto extreme = chromosome({std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max()});
    Smoothing::restoreSmoothness<int, double>(extreme, std::numeric_limits<int>::max(),
                                              std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max(),
                                              false, 0);
    BOOST_CHECK_EQUAL(std::as_const(*extreme).getContainer().back(), -1);
}

BOOST_AUTO_TEST_CASE(peaks_taper_symmetrically_at_both_edges_and_include_the_last_gene)
{
    for (const unsigned int position : {0u, 2u, 4u})
    {
        auto target = chromosome({0, 0, 0, 0, 0});
        Smoothing::peakAt<int, double>(position, 2, 2, 10, target);
        for (unsigned int index = 0; index < 5; ++index)
        {
            const auto distance = index > position ? index - position : position - index;
            const int expected = distance > 2 ? 0 : 10 - 5 * distance;
            BOOST_CHECK_EQUAL(std::as_const(*target).getContainer()[index], expected);
        }
    }
    auto target = chromosome({0, 0, 0});
    Smoothing::peakAt<int, double>(1, 0, 0, 10, target);
    BOOST_CHECK(std::as_const(*target).getContainer() == std::vector<int>({0, 10, 0}));
    Smoothing::peakAt<int, double>(1, std::numeric_limits<unsigned int>::max(),
                                   std::numeric_limits<unsigned int>::max(), 5, target);
    BOOST_CHECK_EQUAL(std::as_const(*target).getContainer()[1], 15);
    auto extreme = chromosome({std::numeric_limits<int>::max()});
    Smoothing::peakAt<int, double>(0, 0, 0, 10, extreme);
    BOOST_CHECK_EQUAL(std::as_const(*extreme).getContainer()[0], std::numeric_limits<int>::max());
}

BOOST_AUTO_TEST_CASE(invalid_smoothing_requests_throw_instead_of_dereferencing_or_indexing_outside_the_genome)
{
    auto empty = chromosome({});
    BOOST_CHECK_NO_THROW((Smoothing::restoreSmoothness<int, double>(empty, 1, 0, 10, false, 0)));
    BOOST_CHECK_THROW((Smoothing::peakAt<int, double>(0, 1, 1, 1, empty)), std::invalid_argument);
    BOOST_CHECK_THROW((Smoothing::peakAt<int, double>(8, 1, 1, 1, chromosome())), std::invalid_argument);
    BOOST_CHECK_THROW((Smoothing::restoreSmoothness<int, double>(nullptr, 1, 0, 10, false, 0)), std::invalid_argument);
    BOOST_CHECK_THROW((Smoothing::restoreSmoothness<int, double>(chromosome(), -1, 0, 10, false, 0)),
                      std::invalid_argument);
    BOOST_CHECK_THROW((Smoothing::restoreSmoothness<int, double>(chromosome(), 1, 10, 0, false, 0)),
                      std::invalid_argument);
    BOOST_CHECK_THROW((Smoothing::restoreSmoothness<int, double>(chromosome(), 1, 0, 10, true, 20)),
                      std::invalid_argument);
    using FloatingChromosome = geneial::population::chromosome::MultiValueChromosome<double, double>;
    auto floatingEvaluator =
        std::make_shared<geneial::FunctionFitnessEvaluator<double>>([](std::span<const double>) { return 0.0; });
    auto floating = std::make_shared<FloatingChromosome>(floatingEvaluator);
    floating->setValueContainer({1, std::numeric_limits<double>::quiet_NaN()});
    BOOST_CHECK_THROW((Smoothing::restoreSmoothness<double, double>(floating, 1, 0, 10, false, 0)),
                      std::invalid_argument);
    BOOST_CHECK_THROW((Smoothing::peakAt<double, double>(0, 2, 2, 1, floating)), std::invalid_argument);
    BOOST_CHECK_EQUAL(std::as_const(*floating).getContainer()[0], 1);
    floating->setValueContainer({1, 2});
    BOOST_CHECK_THROW((Smoothing::peakAt<double, double>(0, 1, 1, std::numeric_limits<double>::quiet_NaN(), floating)),
                      std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(smooth_peak_mutation_is_bounded_cache_preserving_and_dimension_checked)
{
    ContinuousFactory::Builder factoryBuilder(evaluator);
    factoryBuilder.getSettings().setNum(8);
    factoryBuilder.getSettings().setRandomMin(0);
    factoryBuilder.getSettings().setRandomMax(10);
    factoryBuilder.getSettings().setEps(2);
    factoryBuilder.getSettings().setHasStart(true);
    factoryBuilder.getSettings().setStartValue(5);
    auto continuousFactory = std::dynamic_pointer_cast<ContinuousFactory>(factoryBuilder.create());
    SmoothPeakMutationOperation<int, double>::Builder peaks(continuousFactory);
    peaks.setChoosingOperation(geneial::operation::choosing::ChooseRandom<double>::Builder(1).create());
    peaks.getSettings().setMinimumPointsToMutate(2);
    peaks.getSettings().setMaximumPointsToMutate(2);
    peaks.setMaxLeftEps(std::numeric_limits<unsigned int>::max());
    peaks.setMaxRightEps(std::numeric_limits<unsigned int>::max());
    auto original = chromosome();
    original->getFitness();
    original->getHash();
    for (unsigned int iteration = 0; iteration < 50; ++iteration)
    {
        const auto result = peaks.create()->doMutate({original}, *manager);
        BOOST_REQUIRE_EQUAL(result.size(), 1);
        const auto mutated = std::dynamic_pointer_cast<Chromosome>(result.front());
        BOOST_REQUIRE(mutated);
        BOOST_CHECK(mutated != original);
        BOOST_CHECK(original->hasFitness());
        BOOST_CHECK(original->hasCache());
        int previous = 5;
        for (const auto value : std::as_const(*mutated).getContainer())
        {
            BOOST_CHECK(value >= 0 && value <= 10);
            BOOST_CHECK(std::abs(value - previous) <= 2);
            previous = value;
        }
    }
    BOOST_CHECK_THROW(peaks.create()->doMutate({chromosome({1, 2})}, *manager), std::invalid_argument);
    peaks.getSettings().setMaximumPointsToMutate(9);
    BOOST_CHECK_THROW(peaks.create()->doMutate({original}, *manager), std::invalid_argument);
    peaks.getSettings().setMaximumPointsToMutate(2);
    peaks.setMaxElevation(-1);
    BOOST_CHECK_THROW(peaks.create(), std::invalid_argument);
    peaks.setMaxElevation(std::numeric_limits<double>::infinity());
    BOOST_CHECK_THROW(peaks.create(), std::invalid_argument);
    BOOST_CHECK_THROW((SmoothPeakMutationOperation<int, double>::Builder(nullptr).create()), std::invalid_argument);
    using UnsignedFactory = geneial::population::chromosome::ContinousMultiValueChromosomeFactory<unsigned int, double>;
    auto unsignedEvaluator = std::make_shared<geneial::FunctionFitnessEvaluator<unsigned int>>(
        [](std::span<const unsigned int>) { return 0.0; });
    auto unsignedFactory =
        std::dynamic_pointer_cast<UnsignedFactory>(UnsignedFactory::Builder(unsignedEvaluator).create());
    BOOST_CHECK_THROW((SmoothPeakMutationOperation<unsigned int, double>::Builder(unsignedFactory).create()),
                      std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
