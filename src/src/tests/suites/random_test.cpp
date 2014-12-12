/*
 *  Created on: Nov 26, 2014
 *      Author: Benedikt Wolters
 */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utility/random
#include <boost/test/unit_test.hpp>

#include <geneial/utility/random.h>

BOOST_AUTO_TEST_SUITE( RandomSuite )

BOOST_AUTO_TEST_CASE( valid_values_ranges )
{
	using namespace GeneticLibrary::Utility;
	bool myRandBool = random::instance()->generateBit();
	BOOST_CHECK( myRandBool == true || myRandBool == false);
}

//Make TEST_INT_UNIFORM_RUNS, see whether randomness divergates by TEST_INT_UNIFORM_TOLERANCE
#define TEST_INT_UNIFORM_RUNS 1000000
#define TEST_INT_UNIFORM_SLOTS 10
#define TEST_INT_UNIFORM_TOLERANCE 0.01f

BOOST_AUTO_TEST_CASE( int_uniform )
{
	using namespace GeneticLibrary::Utility;

	int occurrences[TEST_INT_UNIFORM_SLOTS];

	for(int i=0; i<(TEST_INT_UNIFORM_SLOTS-1); i++)
	{
		occurrences[i] = 0;
	}

	for(int i=1;i<TEST_INT_UNIFORM_RUNS;i++){
		occurrences[random::instance()->generateInt(0,9)]++;
	}

	for(int i=0; i<(TEST_INT_UNIFORM_SLOTS-1); i++)
	{

		BOOST_CHECK_MESSAGE(
				((float) occurrences[i] / (float) TEST_INT_UNIFORM_RUNS)
					<
				((float) TEST_INT_UNIFORM_RUNS / (float) TEST_INT_UNIFORM_SLOTS) / (float) TEST_INT_UNIFORM_RUNS
				+ (float) TEST_INT_UNIFORM_TOLERANCE,
				"uniform failed for " << i << " with " << (float) occurrences[i] / (float) TEST_INT_UNIFORM_RUNS);
	}
}

BOOST_AUTO_TEST_SUITE_END()
