#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE factory_boundaries
#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif
#include <algorithm>
#include <cstdint>
#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/operations/crossover/SmoothedMultiValueChromosomeNPointCrossover.h>
#include <geneial/core/population/builder/PermutationChromosomeFactory.h>
#include <limits>
#include <numeric>

using namespace geneial::population::chromosome;
namespace
{
template <typename Value> auto objective()
{
    return std::make_shared<geneial::FunctionFitnessEvaluator<Value>>([](std::span<const Value>) { return 0.0; });
}
}

BOOST_AUTO_TEST_CASE(continuous_factories_respect_full_range_signed_and_unsigned_steps)
{
    const auto check = []<typename Value>()
    {
        using Factory = ContinousMultiValueChromosomeFactory<Value, double>;
        typename Factory::Builder builder(objective<Value>());
        auto &settings = builder.getSettings();
        settings.setNum(200);
        settings.setRandomMin(std::numeric_limits<Value>::lowest());
        settings.setRandomMax(std::numeric_limits<Value>::max());
        settings.setEps(std::numeric_limits<Value>::max());
        settings.setHasStart(true);
        const auto factory = builder.create();
        for (const auto anchor : {std::numeric_limits<Value>::lowest(), std::numeric_limits<Value>::max()})
        {
            settings.setStartValue(anchor);
            const auto chromosome =
                std::dynamic_pointer_cast<MultiValueChromosome<Value, double>>(factory->createChromosome());
            auto previous = anchor;
            using Unsigned = std::make_unsigned_t<Value>;
            for (const auto value : std::as_const(*chromosome).getContainer())
            {
                const auto higher = std::max(previous, value);
                const auto lower = std::min(previous, value);
                const auto distance =
                    static_cast<Unsigned>(static_cast<Unsigned>(higher) - static_cast<Unsigned>(lower));
                BOOST_CHECK_LE(distance, static_cast<Unsigned>(settings.getEps()));
                previous = value;
            }
        }
    };
    check.template operator()<std::int64_t>();
    check.template operator()<std::uint64_t>();
}

BOOST_AUTO_TEST_CASE(continuous_factories_validate_limits_even_for_unpopulated_requests)
{
    ContinousMultiValueChromosomeFactory<double, double>::Builder builder(objective<double>());
    auto factory = builder.create();
    auto &settings = builder.getSettings();
    settings.setEps(-1);
    BOOST_CHECK_THROW(factory->createChromosome(), std::invalid_argument);
    settings.setEps(std::numeric_limits<double>::quiet_NaN());
    BOOST_CHECK_THROW(factory->createChromosome(BaseChromosomeFactory<double>::LET_UNPOPULATED), std::invalid_argument);
    settings.setEps(1);
    settings.setHasStart(true);
    settings.setStartValue(settings.getRandomMax() + settings.getEps() + 1);
    BOOST_CHECK_THROW(factory->createChromosome(), std::invalid_argument);
    settings.setHasStart(false);
    settings.setRandomMin(10);
    settings.setRandomMax(0);
    BOOST_CHECK_THROW(factory->createChromosome(), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(empty_and_singleton_factories_terminate_without_shuffle_retries)
{
    PermutationChromosomeFactory<unsigned int, double>::Builder permutation(objective<unsigned int>());
    permutation.getSettings().setPermutationRoundsMin(std::numeric_limits<unsigned int>::max());
    permutation.getSettings().setPermutationRoundsMax(std::numeric_limits<unsigned int>::max());
    for (unsigned int count : {0u, 1u})
    {
        permutation.getSettings().setNum(count);
        auto chromosome = std::dynamic_pointer_cast<MultiValueChromosome<unsigned int, double>>(
            permutation.create()->createChromosome());
        BOOST_CHECK_EQUAL(chromosome->getSize(), count);
        if (count != 0)
            BOOST_CHECK_EQUAL(std::as_const(*chromosome).getContainer().front(), 0);
    }
    ContinousMultiValueChromosomeFactory<int, double>::Builder continuous(objective<int>());
    continuous.getSettings().setNum(0);
    auto chromosome =
        std::dynamic_pointer_cast<MultiValueChromosome<int, double>>(continuous.create()->createChromosome());
    BOOST_CHECK_EQUAL(chromosome->getSize(), 0);
}

BOOST_AUTO_TEST_CASE(permutations_reject_gene_types_that_would_create_duplicate_indices)
{
    PermutationChromosomeFactory<std::uint8_t, double>::Builder small(objective<std::uint8_t>());
    small.getSettings().setNum(256);
    small.getSettings().setPermutationRoundsMin(0);
    small.getSettings().setPermutationRoundsMax(0);
    auto chromosome =
        std::dynamic_pointer_cast<MultiValueChromosome<std::uint8_t, double>>(small.create()->createChromosome());
    BOOST_CHECK_EQUAL(chromosome->getSize(), 256);
    BOOST_CHECK_EQUAL(std::as_const(*chromosome).getContainer().back(), 255);
    small.getSettings().setNum(257);
    BOOST_CHECK_THROW(small.create()->createChromosome(), std::invalid_argument);
    PermutationChromosomeFactory<float, double>::Builder rounded(objective<float>());
    rounded.getSettings().setNum((1u << 24) + 2);
    BOOST_CHECK_THROW(rounded.create()->createChromosome(), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(permutation_settings_have_no_negative_sentinel_and_validate_rounds)
{
    PermutationChromosomeFactory<unsigned int, double>::Builder builder(objective<unsigned int>());
    BOOST_CHECK_EQUAL(builder.getSettings().getRandomMin(), 0);
    BOOST_CHECK_EQUAL(builder.getSettings().getRandomMax(), 0);
    builder.getSettings().setPermutationRoundsMin(5);
    builder.getSettings().setPermutationRoundsMax(4);
    BOOST_CHECK_THROW(builder.create()->createChromosome(), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(smoothed_crossover_rejects_incompatible_factories_and_degenerate_parents)
{
    using Crossover = geneial::operation::crossover::SmoothedMultiValueChromosomeNPointCrossover<int, double>;
    auto plain = std::dynamic_pointer_cast<MultiValueChromosomeFactory<int, double>>(
        MultiValueChromosomeFactory<int, double>::Builder(objective<int>()).create());
    Crossover::Builder invalid(plain);
    BOOST_CHECK_THROW(invalid.create(), std::invalid_argument);
    ContinousMultiValueChromosomeFactory<int, double>::Builder builder(objective<int>());
    builder.getSettings().setNum(1);
    auto factory = std::dynamic_pointer_cast<ContinousMultiValueChromosomeFactory<int, double>>(builder.create());
    Crossover::Builder valid(factory);
    BOOST_CHECK_THROW(valid.create(), std::invalid_argument);
    builder.getSettings().setNum(8);
    auto operation = std::dynamic_pointer_cast<Crossover>(valid.create());
    const auto first = std::dynamic_pointer_cast<MultiValueChromosome<int, double>>(factory->createChromosome());
    const auto second = std::dynamic_pointer_cast<MultiValueChromosome<int, double>>(factory->createChromosome());
    const auto children = operation->doMultiValueCrossover(first, second);
    BOOST_REQUIRE(!children.empty());
    for (const auto &candidate : children)
    {
        const auto genes = std::dynamic_pointer_cast<MultiValueChromosome<int, double>>(candidate);
        const auto &values = std::as_const(*genes).getContainer();
        for (std::size_t index = 1; index < values.size(); ++index)
            BOOST_CHECK_LE(std::abs(values[index] - values[index - 1]), builder.getSettings().getEps());
    }
}
