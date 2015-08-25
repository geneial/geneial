#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE algorithm/crossover

#include <boost/test/unit_test.hpp>

#include <geneial/core/operations/coupling/LambdaCouplingAdapter.h>
#include <geneial/core/operations/crossover/BaseCrossoverOperation.h>
#include <geneial/core/population/management/BaseManager.h>

BOOST_AUTO_TEST_SUITE( __CRITERIA_TEST_CombinedCriterion )
BOOST_AUTO_TEST_CASE( LambdaAdapterTest )
{
    BOOST_CHECK(1==1);
}
BOOST_AUTO_TEST_SUITE_END()
