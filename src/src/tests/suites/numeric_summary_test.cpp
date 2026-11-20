#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE numeric_summary
#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif

#include <algorithm>
#include <array>
#include <cstdint>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <limits>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

#include "mocks/MockFitnessEvaluator.h"

namespace
{
template <typename Value> auto chromosome(std::vector<Value> values)
{
    geneial::population::chromosome::MultiValueChromosome<Value, double> result(
        std::make_shared<test_mock::MockFitnessEvaluator<double>>());
    result.setValueContainer(std::move(values));
    return result;
}

template <typename Value> void checkSignedSummaries()
{
    const auto maximum = std::numeric_limits<Value>::max();
    const auto minimum = std::numeric_limits<Value>::lowest();
    BOOST_CHECK_EQUAL(chromosome<Value>({maximum, maximum}).getAverage(), maximum);
    BOOST_CHECK_EQUAL(chromosome<Value>({minimum, minimum}).getAverage(), minimum);
    BOOST_CHECK_EQUAL(chromosome<Value>({maximum, static_cast<Value>(maximum - 1)}).getAverage(), maximum - 1);
    BOOST_CHECK_EQUAL(chromosome<Value>({minimum, static_cast<Value>(minimum + 1)}).getAverage(), minimum + 1);
    BOOST_CHECK_EQUAL(chromosome<Value>({minimum, maximum}).getAverage(), 0);
    BOOST_CHECK_EQUAL(chromosome<Value>({minimum, maximum}).getSum(), -1);
    BOOST_CHECK_EQUAL(chromosome<Value>({maximum, maximum, minimum}).getSum(), maximum - 1);
    BOOST_CHECK_EQUAL(chromosome<Value>({minimum, minimum, maximum, 1}).getSum(), minimum);
    BOOST_CHECK_THROW(chromosome<Value>({minimum, maximum}).getRange(), std::overflow_error);
    BOOST_CHECK_THROW(chromosome<Value>({maximum, 1}).getSum(), std::overflow_error);
    BOOST_CHECK_THROW(chromosome<Value>({minimum, -1}).getSum(), std::overflow_error);
    BOOST_CHECK_THROW(chromosome<Value>({maximum, maximum, -1}).getSum(), std::overflow_error);
    BOOST_CHECK_THROW(chromosome<Value>({minimum, minimum, maximum}).getSum(), std::overflow_error);

    const std::array<Value, 3> genes{minimum, 1, maximum};
    std::array<unsigned int, 6> order{0, 0, 1, 1, 2, 2};
    do
    {
        std::vector<Value> values;
        for (const auto index : order)
            values.push_back(genes[index]);
        const auto candidate = chromosome(std::move(values));
        BOOST_CHECK_EQUAL(candidate.getSum(), 0);
        BOOST_CHECK_EQUAL(candidate.getAverage(), 0);
    } while (std::next_permutation(order.begin(), order.end()));
}

template <typename Value> void checkUnsignedSummaries()
{
    const auto maximum = std::numeric_limits<Value>::max();
    BOOST_CHECK_EQUAL(chromosome<Value>({maximum, maximum}).getAverage(), maximum);
    BOOST_CHECK_EQUAL(chromosome<Value>({maximum, static_cast<Value>(maximum - 1)}).getAverage(), maximum - 1);
    BOOST_CHECK_EQUAL(chromosome<Value>({0, maximum}).getRange(), maximum);
    BOOST_CHECK_EQUAL(chromosome<Value>({static_cast<Value>(maximum - 1), 1}).getSum(), maximum);
    BOOST_CHECK_THROW(chromosome<Value>({maximum, 1}).getSum(), std::overflow_error);
    BOOST_CHECK_THROW(chromosome<Value>({maximum, maximum}).getSum(), std::overflow_error);
}
}

BOOST_AUTO_TEST_CASE(signed_summaries_are_exact_at_every_integer_width)
{
    checkSignedSummaries<std::int8_t>();
    checkSignedSummaries<std::int16_t>();
    checkSignedSummaries<std::int32_t>();
    checkSignedSummaries<std::int64_t>();
}

BOOST_AUTO_TEST_CASE(unsigned_summaries_do_not_wrap_at_any_integer_width)
{
    checkUnsignedSummaries<std::uint8_t>();
    checkUnsignedSummaries<std::uint16_t>();
    checkUnsignedSummaries<std::uint32_t>();
    checkUnsignedSummaries<std::uint64_t>();
}

BOOST_AUTO_TEST_CASE(averages_truncate_toward_zero_after_cancelling_remainders)
{
    BOOST_CHECK_EQUAL(chromosome<int>({1, 1, 1, -2}).getAverage(), 0);
    BOOST_CHECK_EQUAL(chromosome<int>({-1, -1, -1, 2}).getAverage(), 0);
    BOOST_CHECK_EQUAL(chromosome<int>({5, 5, -1}).getAverage(), 3);
    BOOST_CHECK_EQUAL(chromosome<int>({-5, -5, 1}).getAverage(), -3);
    BOOST_CHECK_EQUAL(chromosome<int>({2, 2, -1}).getAverage(), 1);
    BOOST_CHECK_EQUAL(chromosome<int>({-2, -2, 1}).getAverage(), -1);
    BOOST_CHECK_EQUAL(chromosome<std::uint8_t>(std::vector<std::uint8_t>(1024, 255)).getAverage(), 255);
    BOOST_CHECK_EQUAL(chromosome<std::int8_t>(std::vector<std::int8_t>(1024, -128)).getAverage(), -128);
    const auto maximum = std::numeric_limits<std::int64_t>::max();
    const auto minimum = std::numeric_limits<std::int64_t>::lowest();
    BOOST_CHECK_EQUAL(chromosome<std::int64_t>({maximum, maximum, -1}).getAverage(), 6148914691236517204LL);
    BOOST_CHECK_EQUAL(chromosome<std::int64_t>({minimum, minimum, maximum}).getAverage(), -3074457345618258603LL);
    BOOST_CHECK_EQUAL(chromosome<std::uint64_t>({UINT64_MAX, UINT64_MAX, 0}).getAverage(), 12297829382473034410ULL);
}

BOOST_AUTO_TEST_CASE(boolean_summaries_use_the_same_integral_contract)
{
    BOOST_CHECK_EQUAL(chromosome<bool>({true, false}).getAverage(), false);
    BOOST_CHECK_EQUAL(chromosome<bool>({true, true}).getAverage(), true);
    BOOST_CHECK_EQUAL(chromosome<bool>({true, false}).getSum(), true);
    BOOST_CHECK_EQUAL(chromosome<bool>({false, true}).getRange(), true);
    BOOST_CHECK_THROW(chromosome<bool>({true, true}).getSum(), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(empty_singleton_and_floating_summaries_remain_compatible)
{
    const auto empty = chromosome<std::int64_t>({});
    BOOST_CHECK_EQUAL(empty.getAverage(), 0);
    BOOST_CHECK_EQUAL(empty.getSum(), 0);
    BOOST_CHECK_EQUAL(empty.getRange(), 0);
    const auto singleton = chromosome<std::int64_t>({std::numeric_limits<std::int64_t>::lowest()});
    BOOST_CHECK_EQUAL(singleton.getSum(), std::numeric_limits<std::int64_t>::lowest());
    BOOST_CHECK_EQUAL(singleton.getAverage(), std::numeric_limits<std::int64_t>::lowest());
    BOOST_CHECK_EQUAL(singleton.getRange(), 0);
    const auto floating = chromosome<double>({-3.5, 1.0, 4.0});
    BOOST_CHECK_EQUAL(floating.getSum(), 1.5);
    BOOST_CHECK_EQUAL(floating.getAverage(), 0.5);
    BOOST_CHECK_EQUAL(floating.getRange(), 7.5);
}

BOOST_AUTO_TEST_CASE(integer_summaries_match_a_wider_reference_over_seeded_inputs)
{
    const auto check = []<typename Value>()
    {
        std::mt19937 engine(42);
        std::uniform_int_distribution<int> length(0, 100);
        std::uniform_int_distribution<Value> gene(std::numeric_limits<Value>::lowest(),
                                                  std::numeric_limits<Value>::max());
        for (unsigned int trial = 0; trial < 1000; ++trial)
        {
            std::vector<Value> values(static_cast<std::size_t>(length(engine)));
            for (auto &value : values)
                value = gene(engine);
            const auto sum = std::accumulate(values.begin(), values.end(), std::int64_t{});
            const auto candidate = chromosome(values);
            BOOST_CHECK_EQUAL(candidate.getAverage(),
                              values.empty() ? 0 : sum / static_cast<std::int64_t>(values.size()));
            if (sum < std::numeric_limits<Value>::lowest() || sum > std::numeric_limits<Value>::max())
                BOOST_CHECK_THROW(candidate.getSum(), std::overflow_error);
            else
                BOOST_CHECK_EQUAL(candidate.getSum(), sum);
            const auto [minimum, maximum] = std::minmax_element(values.begin(), values.end());
            const auto range = values.empty() ? 0 : static_cast<std::int64_t>(*maximum) - *minimum;
            if (range > std::numeric_limits<Value>::max())
                BOOST_CHECK_THROW(candidate.getRange(), std::overflow_error);
            else
                BOOST_CHECK_EQUAL(candidate.getRange(), range);
        }
    };
    check.operator()<std::int32_t>();
    check.operator()<std::uint32_t>();
}
