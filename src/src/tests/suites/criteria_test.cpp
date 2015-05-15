#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE algorithm/criteria

#include <boost/test/unit_test.hpp>

#include <geneial/algorithm/criteria/CombinedCriterion.h>

#include <geneial/algorithm/criteria/NegationDecorator.h>
#include <geneial/algorithm/criteria/ConsecutiveDecorator.h>

#include <geneial/algorithm/criteria/FitnessValueDeltaCriterion.h>
#include <geneial/algorithm/criteria/FitnessValueReachedCriterion.h>
#include <geneial/algorithm/criteria/PopulationUnchangedCriterion.h>
#include <geneial/algorithm/criteria/TrailingStopCriterion.h>
#include <geneial/algorithm/criteria/FixPointCriterion.h>

//TODO(bewo): Write this tests..

BOOST_AUTO_TEST_SUITE( __CRITERIA_TEST_CombinedCriterion )
BOOST_AUTO_TEST_CASE( Continuity_Factory )
{
    BOOST_CHECK(1==1); //alibi
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( __CRITERIA_TEST_NegationDecorator )
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( __CRITERIA_TEST_ConsecutiveDecorator )
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( __CRITERIA_TEST_FitnessValueDeltaCriterion )
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( __CRITERIA_TEST_FitnessValueReachedCriterion )
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( __CRITERIA_TEST_PopulationUnchangedCriterion )
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( __CRITERIA_TEST_TrailingStopCriterion )
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( __CRITERIA_TEST_FixPointCriterion )
BOOST_AUTO_TEST_SUITE_END()

