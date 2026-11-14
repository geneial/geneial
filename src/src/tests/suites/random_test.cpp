#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE random
#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif
#include <array>
#include <geneial/utility/Random.h>
#include <limits>

using geneial::utility::Random;

struct Seeded
{
    Seeded()
    {
        Random::initialize(42);
    }
};
BOOST_FIXTURE_TEST_SUITE(RandomSuite, Seeded)

BOOST_AUTO_TEST_CASE(integer_and_real_distributions_cover_their_intervals)
{
    std::array<unsigned int, 10> integers{}, reals{};
    for (unsigned int trial = 0; trial < 100000; ++trial)
    {
        const int integer = Random::generate(-5, 4);
        const double real = Random::generate(-5.0, 5.0);
        BOOST_REQUIRE(integer >= -5 && integer <= 4);
        BOOST_REQUIRE(real >= -5 && real < 5);
        ++integers[integer + 5];
        ++reals[static_cast<unsigned int>(real + 5)];
    }
    for (const auto &distribution : {integers, reals})
        for (const auto count : distribution)
            BOOST_CHECK(count > 9000 && count < 11000);
}

BOOST_AUTO_TEST_CASE(small_integral_types_and_degenerate_ranges_are_supported)
{
    unsigned int heads = 0;
    for (unsigned int trial = 0; trial < 10000; ++trial)
    {
        heads += Random::generate<bool>();
        const auto signedValue = Random::generate<signed char>(-4, 4);
        const auto unsignedValue = Random::generate<unsigned char>(0, 255);
        BOOST_CHECK(signedValue >= -4 && signedValue <= 4);
        BOOST_CHECK(unsignedValue <= 255);
        BOOST_CHECK_EQUAL(Random::generate(7, 7), 7);
        BOOST_CHECK_EQUAL(Random::generate(-1.5, -1.5), -1.5);
    }
    BOOST_CHECK(heads > 4500 && heads < 5500);
    BOOST_CHECK(!Random::decision(0));
    BOOST_CHECK(Random::decision(1));
}

BOOST_AUTO_TEST_CASE(default_real_draws_exclude_the_upper_endpoint)
{
    for (unsigned int trial = 0; trial < 10000; ++trial)
    {
        const auto single = Random::generate<float>();
        const auto extended = Random::generate<long double>();
        BOOST_CHECK(single >= 0 && single < 1);
        BOOST_CHECK(extended >= 0 && extended < 1);
    }
}
BOOST_AUTO_TEST_SUITE_END()
