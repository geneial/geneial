#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif

#define BOOST_TEST_MODULE algorithm/crossover

#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif


BOOST_AUTO_TEST_SUITE( __COUPLING_TEST_LambdaAdapterTest )
BOOST_AUTO_TEST_CASE( LambdaAdapterTest )
{
    BOOST_CHECK(1==1);
}
BOOST_AUTO_TEST_SUITE_END()
