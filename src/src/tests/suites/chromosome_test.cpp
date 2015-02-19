/*
 *  Created on: Nov 26, 2014
 *      Author: Benedikt Wolters
 */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE chromomsome

#include <boost/test/unit_test.hpp>
#include <set>
#include <vector>

#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>

#include "mocks/MockFitnessEvaluator.h"
#include "mocks/MockMultivalueChromosome.h"

BOOST_AUTO_TEST_SUITE( ChromosomeTest )


BOOST_AUTO_TEST_CASE( HASHMVC___no_hash_sideeffects )
{
	const int a[] = {1, 2, 3, 4, 5};
	const std::vector<int> contentsInt(a,a+sizeof(a)/sizeof(int));
	MultiValueChromosome<int,double>::const_ptr ptrA = getMockIntMVC(contentsInt);
	MultiValueChromosome<int,double>::chromsome_hash hash1 = ptrA->getHash();
	MultiValueChromosome<int,double>::chromsome_hash hash2 = ptrA->getHash();
	BOOST_CHECK(hash1 == hash2);

	const double d[] = {1, 2, 3, 4, 5};
	const std::vector<double> contentsDouble(d,d+sizeof(d)/sizeof(double));
	MultiValueChromosome<double,double>::const_ptr ptrAd = getMockDoubleMVC(contentsDouble);
	MultiValueChromosome<int,double>::chromsome_hash hashD1 = ptrAd->getHash();
	MultiValueChromosome<int,double>::chromsome_hash hashD2 = ptrAd->getHash();
	BOOST_CHECK(hashD1 == hashD2);


}

BOOST_AUTO_TEST_CASE( HASHMVC___same_hash )
{
	const int a[] = {1, 2, 3, 4, 5};
	const std::vector<int> contentsInt(a,a+sizeof(a)/sizeof(int));

	const double d[] = {1, 2, 3, 4, 5};
	const std::vector<double> contentsDouble(d,d+sizeof(d)/sizeof(double));

	MultiValueChromosome<int,double>::const_ptr ptrA = getMockIntMVC(contentsInt);
	MultiValueChromosome<int,double>::const_ptr ptrB = getMockIntMVC(contentsInt);
	BOOST_CHECK(ptrA != ptrB);
	BOOST_CHECK(ptrA->getHash() == ptrB->getHash());

	MultiValueChromosome<double,double>::const_ptr ptrAd = getMockDoubleMVC(contentsDouble);
	MultiValueChromosome<double,double>::const_ptr ptrBd = getMockDoubleMVC(contentsDouble);
	BOOST_CHECK(ptrAd != ptrBd);
	BOOST_CHECK(ptrAd->getHash() == ptrBd->getHash());


}

BOOST_AUTO_TEST_CASE( HASHMVC___different_hash )
{
	const int a1[] = {1, 2, 3, 4, 5};
	const int a2[] = {1, 1, 3, 4, 5};
	const std::vector<int> contentsInt1(a1,a1+sizeof(a1)/sizeof(int));
	const std::vector<int> contentsInt2(a2,a2+sizeof(a2)/sizeof(int));


	const double d1[] = {1, 2, 3, 4, 5};
	const double d2[] = {1, 1, 3, 4, 5};
	const std::vector<double> contentsDouble1(d1,d2+sizeof(d2)/sizeof(double));
	const std::vector<double> contentsDouble2(d2,d2+sizeof(d2)/sizeof(double));

	MultiValueChromosome<int,double>::const_ptr ptrA = getMockIntMVC(contentsInt1);
	MultiValueChromosome<int,double>::const_ptr ptrB = getMockIntMVC(contentsInt2);
	BOOST_CHECK(ptrA != ptrB);
	BOOST_CHECK(ptrA->getHash() != ptrB->getHash());

	MultiValueChromosome<double,double>::const_ptr ptrAd = getMockDoubleMVC(contentsDouble1);
	MultiValueChromosome<double,double>::const_ptr ptrBd = getMockDoubleMVC(contentsDouble2);
	BOOST_CHECK(ptrAd != ptrBd);
	BOOST_CHECK(ptrAd->getHash() != ptrBd->getHash());
}

#define COLLISION_ROBUSTNESS  (10000) // (100000000)
BOOST_AUTO_TEST_CASE( HASHMVC___no_collisions )
{
	std::set<MultiValueChromosome<int,double>::chromsome_hash> setHashes;
	for(int i =0; i< COLLISION_ROBUSTNESS;i++){
		const int a[] = {i,i+1,i+2,-i,i-1,-2};
		const std::vector<int> contentsInt1(a,a+sizeof(a)/sizeof(int));
		MultiValueChromosome<int,double>::const_ptr ptrA = getMockIntMVC(contentsInt1);
		BOOST_CHECK(setHashes.find(ptrA->getHash()) == setHashes.end());
		setHashes.insert(ptrA->getHash());
	}
}

BOOST_AUTO_TEST_SUITE_END()
