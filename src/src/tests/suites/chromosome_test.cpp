#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE test/chromomsome

#include <geneial/core/population/chromosome/MultiValueChromosome.h>

#include <boost/test/unit_test.hpp>
#include <set>
#include <vector>

#include "mocks/MockMultivalueChromosome.h"

using namespace test_mock;

using ::geneial::population::chromosome::MultiValueChromosome;

BOOST_AUTO_TEST_SUITE( ChromosomeTest )

BOOST_AUTO_TEST_CASE( HASHMVC___no_hash_sideeffects )
{
    const std::vector<int> contentsInt =  { 1, 2, 3, 4, 5 };
    MultiValueChromosome<int, double>::const_ptr ptrA = getMockIntMVC(contentsInt);
    MultiValueChromosome<int, double>::chromsome_hash hash1 = ptrA->getHash();
    MultiValueChromosome<int, double>::chromsome_hash hash2 = ptrA->getHash();
    BOOST_CHECK(hash1 == hash2);

    const std::vector<double> contentsDouble = { 1, 2, 3, 4, 5 };
    MultiValueChromosome<double, double>::const_ptr ptrAd = getMockDoubleMVC(contentsDouble);
    MultiValueChromosome<int, double>::chromsome_hash hashD1 = ptrAd->getHash();
    MultiValueChromosome<int, double>::chromsome_hash hashD2 = ptrAd->getHash();
    BOOST_CHECK(hashD1 == hashD2);

}

BOOST_AUTO_TEST_CASE( HASHMVC___same_hash )
{
    const std::vector<int> contentsInt = { 1, 2, 3, 4, 5 };

    const std::vector<double> contentsDouble = { 1, 2, 3, 4, 5 };

    MultiValueChromosome<int, double>::const_ptr ptrA = getMockIntMVC(contentsInt);
    MultiValueChromosome<int, double>::const_ptr ptrB = getMockIntMVC(contentsInt);
    BOOST_CHECK(ptrA != ptrB);
    BOOST_CHECK(ptrA->getHash() == ptrB->getHash());

    MultiValueChromosome<double, double>::const_ptr ptrAd = getMockDoubleMVC(contentsDouble);
    MultiValueChromosome<double, double>::const_ptr ptrBd = getMockDoubleMVC(contentsDouble);
    BOOST_CHECK(ptrAd != ptrBd);
    BOOST_CHECK(ptrAd->getHash() == ptrBd->getHash());

}

BOOST_AUTO_TEST_CASE( HASHMVC___different_hash )
{
    const std::vector<int> contentsInt1 = { 1, 2, 3, 4, 5 };
    const std::vector<int> contentsInt2 = { 1, 1, 3, 4, 5 };

    const std::vector<double> contentsDouble1 = { 1, 2, 3, 4, 5 };
    const std::vector<double> contentsDouble2 = { 1, 1, 3, 4, 5 };

    MultiValueChromosome<int, double>::const_ptr ptrA = getMockIntMVC(contentsInt1);
    MultiValueChromosome<int, double>::const_ptr ptrB = getMockIntMVC(contentsInt2);
    BOOST_CHECK(ptrA != ptrB);
    BOOST_CHECK(ptrA->getHash() != ptrB->getHash());

    MultiValueChromosome<double, double>::const_ptr ptrAd = getMockDoubleMVC(contentsDouble1);
    MultiValueChromosome<double, double>::const_ptr ptrBd = getMockDoubleMVC(contentsDouble2);
    BOOST_CHECK(ptrAd != ptrBd);
    BOOST_CHECK(ptrAd->getHash() != ptrBd->getHash());
}

#define COLLISION_ROBUSTNESS  (10000) // (100000000)
BOOST_AUTO_TEST_CASE( HASHMVC___no_collisions )
{
    std::set<MultiValueChromosome<int, double>::chromsome_hash> setHashes;
    for (int i = 0; i < COLLISION_ROBUSTNESS; i++)
    {
        const std::vector<int> contentsInt1 = { i, i + 1, i + 2, -i, i - 1, -2 };
        MultiValueChromosome<int, double>::const_ptr ptrA = getMockIntMVC(contentsInt1);
        BOOST_CHECK(setHashes.find(ptrA->getHash()) == setHashes.end());
        setHashes.insert(ptrA->getHash());
    }
}

BOOST_AUTO_TEST_SUITE_END()
