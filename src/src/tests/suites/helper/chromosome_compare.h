#pragma once

#include <geneial/core/population/chromosome/MultiValueChromosome.h>

#include "common_casts.h"
using namespace geneial::population::chromosome;

namespace test_helper
{
/**
 * Compares two mvc chromosomes sizewise
 * @param chromosomeToCheckA
 * @param chromosomeToCheckB
 * @return true if size equals
 */
template<typename VALUE_TYPE, typename FITNESS_TYPE>
inline bool chromosomeSizeCompare(const typename BaseChromosome<FITNESS_TYPE>::ptr chromosomeToCheckA,
                                  const typename BaseChromosome<FITNESS_TYPE>::ptr chromosomeToCheckB)
{
    const auto ptrA = test_helper::convertBaseChromosome<VALUE_TYPE,FITNESS_TYPE>(chromosomeToCheckA);
    const auto ptrB = test_helper::convertBaseChromosome<VALUE_TYPE,FITNESS_TYPE>(chromosomeToCheckB);
    return ptrA->getSize() == ptrB->getSize();
}


/**
 * Compares the values of two chromosomes
 * @param chromosomeToCheckA
 * @param chromosomeToCheckB
 * @return true, if both equals
 */
template<typename VALUE_TYPE, typename FITNESS_TYPE>
inline bool chromosomeContentCompare(const typename BaseChromosome<FITNESS_TYPE>::ptr chromosomeToCheckA,
                                     const typename BaseChromosome<FITNESS_TYPE>::ptr chromosomeToCheckB)
{
    const auto ptrA = test_helper::convertBaseChromosome<VALUE_TYPE,FITNESS_TYPE>(chromosomeToCheckA);
    const auto ptrB = test_helper::convertBaseChromosome<VALUE_TYPE,FITNESS_TYPE>(chromosomeToCheckB);

    return std::equal(ptrA->getContainer().begin(), ptrA->getContainer().end(), ptrB->getContainer().begin())
        && std::equal(ptrB->getContainer().begin(), ptrB->getContainer().end(), ptrA->getContainer().begin());
}


/**
 * Check whether two chromosomes are equal, NOT using geneial's internal chromosome hashing.
 * @param chromosomeToCheckA
 * @param chromosomeToCheckB
 * @return true, if chromosomes are different
 */
template<typename VALUE_TYPE, typename FITNESS_TYPE>
inline bool chromosomeCompare(const typename BaseChromosome<FITNESS_TYPE>::ptr chromosomeToCheckA,
                              const typename BaseChromosome<FITNESS_TYPE>::ptr chromosomeToCheckB)
{
    bool compareResult = chromosomeSizeCompare<VALUE_TYPE,FITNESS_TYPE>(chromosomeToCheckA,chromosomeToCheckB);
         compareResult = chromosomeContentCompare<VALUE_TYPE,FITNESS_TYPE>(chromosomeToCheckA,chromosomeToCheckB);
    return compareResult;
}


/**
 * Check whether two chromosomes are equal by using geneial's internal chromosome hashing.
 * @param chromosomeToCheckA
 * @param chromosomeToCheckB
 * @return true, if hashes are equal
 */
template<typename VALUE_TYPE, typename FITNESS_TYPE>
inline bool chromosomeHashCompare(const typename BaseChromosome<FITNESS_TYPE>::ptr chromosomeToCheckA,
                                  const typename BaseChromosome<FITNESS_TYPE>::ptr chromosomeToCheckB)
{
    return chromosomeToCheckA->getHash() == chromosomeToCheckB->getHash();
}

}
