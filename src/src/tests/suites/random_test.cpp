/*
 *  Created on: Nov 26, 2014
 *      Author: Benedikt Wolters
 */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utility/random
#include <boost/test/unit_test.hpp>

#include <geneial/utility/Random.h>
using namespace GeneticLibrary::Utility;

BOOST_AUTO_TEST_SUITE( RandomSuite )

BOOST_AUTO_TEST_CASE( valid_values_ranges_bool )
{
	bool myRandBool = Random::instance()->generateBit();
	BOOST_CHECK( myRandBool == true || myRandBool == false);
}

#define TEST_INT_VALUERANGE_RUNS 1000
BOOST_AUTO_TEST_CASE( valid_values_ranges_int )
{
	for(int i=1;i<TEST_INT_VALUERANGE_RUNS;i++){
		const int result = Random::instance()->generateInt(0,100);
		BOOST_CHECK( result >= 0 && result <= 100);
	}

	for(int i=1;i<TEST_INT_VALUERANGE_RUNS;i++){
		const int result = Random::instance()->generateInt(-100,0);
		BOOST_CHECK( result >= -100 && result <= 0);
	}

	for(int i=1;i<TEST_INT_VALUERANGE_RUNS;i++){
		const int result = Random::instance()->generateInt(-100,100);
		BOOST_CHECK( result >= -100 && result <= 100);
	}


	for(int i=1;i<TEST_INT_VALUERANGE_RUNS;i++){
		const float result = Random::instance()->generateFloat(0,100);
		BOOST_CHECK( result >= 0 && result <= 100);
	}

	for(int i=1;i<TEST_INT_VALUERANGE_RUNS;i++){
		const float result = Random::instance()->generateFloat(-100,0);
		BOOST_CHECK( result >= -100 && result <= 0);
	}

	for(int i=1;i<TEST_INT_VALUERANGE_RUNS;i++){
		const float result = Random::instance()->generateFloat(-100,100);
		BOOST_CHECK( result >= -100 && result <= 100);
	}

	for(int i=1;i<TEST_INT_VALUERANGE_RUNS;i++){
		const double result = Random::instance()->generateDouble(0,100);
		BOOST_CHECK( result >= 0 && result <= 100);
	}

	for(int i=1;i<TEST_INT_VALUERANGE_RUNS;i++){
		const double result = Random::instance()->generateDouble(-100,0);
		BOOST_CHECK( result >= -100 && result <= 0);
	}

	for(int i=1;i<TEST_INT_VALUERANGE_RUNS;i++){
		const double result = Random::instance()->generateDouble(-100,100);
		BOOST_CHECK( result >= -100 && result <= 100);
	}
}

BOOST_AUTO_TEST_CASE( difference )
{
	double myRand1D= Random::instance()->generateDouble();
	double myRand2D= Random::instance()->generateDouble();
	BOOST_CHECK( myRand1D != myRand2D);

	float myRand1F= Random::instance()->generateFloat();
	float myRand2F= Random::instance()->generateFloat();
	BOOST_CHECK( myRand1F != myRand2F);


	int myRand1I= Random::instance()->generateInt();
	int myRand2I= Random::instance()->generateInt();
	BOOST_CHECK( myRand1I != myRand2I);
}

//Make TEST_INT_UNIFORM_RUNS, see whether randomness divergates by TEST_INT_UNIFORM_TOLERANCE
#define TEST_INT_UNIFORM_RUNS 1e6
#define TEST_INT_UNIFORM_SLOTS 10
#define TEST_INT_UNIFORM_TOLERANCE 0.001f

BOOST_AUTO_TEST_CASE( int_uniform )
{

	BOOST_TEST_MESSAGE("TEST_INT_UNIFORM_RUNS: " << TEST_INT_UNIFORM_RUNS);
	BOOST_TEST_MESSAGE("TEST_INT_UNIFORM_SLOTS: " << TEST_INT_UNIFORM_SLOTS);
	BOOST_TEST_MESSAGE("TEST_INT_UNIFORM_TOLERANCE: " << TEST_INT_UNIFORM_TOLERANCE);

	int occurrences[TEST_INT_UNIFORM_SLOTS];

	for(int i=0; i<(TEST_INT_UNIFORM_SLOTS); i++)
	{
		occurrences[i] = 0;
	}

	for(int i=1;i<TEST_INT_UNIFORM_RUNS;i++){
		occurrences[Random::instance()->generateInt(0,TEST_INT_UNIFORM_SLOTS-1)]++;
	}

	BOOST_TEST_MESSAGE("Int Distribution: ");
	for(int i=0; i<(TEST_INT_UNIFORM_SLOTS); i++)
	{
		BOOST_TEST_MESSAGE(i << " " << std::string((int)100* ((float) occurrences[i] / (float) TEST_INT_UNIFORM_RUNS), '+') );
	}

	for(int i=0; i<(TEST_INT_UNIFORM_SLOTS); i++)
	{
		BOOST_CHECK_MESSAGE(
				((float) occurrences[i] / (float) TEST_INT_UNIFORM_RUNS)
					<
				((float) TEST_INT_UNIFORM_RUNS / (float) TEST_INT_UNIFORM_SLOTS) / (float) TEST_INT_UNIFORM_RUNS
				+ (float) TEST_INT_UNIFORM_TOLERANCE,
				"uniform failed for " << i << " with " << (float) occurrences[i] / (float) TEST_INT_UNIFORM_RUNS);
	}
}
#define TEST_DOUBLE_UNIFORM_RUNS 1e6
#define TEST_DOUBLE_UNIFORM_SLOTS 10
#define TEST_DOUBLE_UNIFORM_TOLERANCE 0.001f
#define TEST_DOUBLE_BOUND_TOLERANCE 0.00001f
BOOST_AUTO_TEST_CASE( double_uniform )
{

	BOOST_TEST_MESSAGE("TEST_DOUBLE_UNIFORM_RUNS: " << TEST_DOUBLE_UNIFORM_RUNS);
	BOOST_TEST_MESSAGE("TEST_DOUBLE_UNIFORM_SLOTS: " << TEST_DOUBLE_UNIFORM_SLOTS);
	BOOST_TEST_MESSAGE("TEST_DOUBLE_UNIFORM_TOLERANCE: " << TEST_DOUBLE_UNIFORM_TOLERANCE);

	int occurrences[TEST_DOUBLE_UNIFORM_SLOTS];

	for(int i=0; i<(TEST_DOUBLE_UNIFORM_SLOTS); i++)
	{
		occurrences[i] = 0;
	}

	for(int i=1;i<TEST_DOUBLE_UNIFORM_RUNS;i++){
		const int val = (int)Random::instance()->generateDouble(0,TEST_DOUBLE_UNIFORM_SLOTS-TEST_DOUBLE_BOUND_TOLERANCE);
		occurrences[val]++;
	}

	BOOST_TEST_MESSAGE("Double Distribution: ");
	for(int i=0; i<(TEST_DOUBLE_UNIFORM_SLOTS); i++)
	{
		BOOST_TEST_MESSAGE(i << " " << std::string((int)100* ((float) occurrences[i] / (float) TEST_DOUBLE_UNIFORM_RUNS), '+') );
	}

	for(int i=0; i<(TEST_DOUBLE_UNIFORM_SLOTS); i++)
	{
		BOOST_CHECK_MESSAGE(
				((float) occurrences[i] / (float) TEST_DOUBLE_UNIFORM_RUNS)
					<
				((float) TEST_DOUBLE_UNIFORM_RUNS / (float) TEST_DOUBLE_UNIFORM_SLOTS) / (float) TEST_DOUBLE_UNIFORM_RUNS
				+ (float) TEST_DOUBLE_UNIFORM_TOLERANCE,
				"uniform failed for " << i << " with " << (float) occurrences[i] / (float) TEST_DOUBLE_UNIFORM_RUNS);
	}
}

BOOST_AUTO_TEST_SUITE_END()
