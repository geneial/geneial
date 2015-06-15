#pragma once

#include <geneial/core/population/chromosome/MultiValueChromosome.h>

using namespace geneial::population::chromosome;

namespace test_helper
{

/**
 * Checks whether a given chromosome still is continous
 * @param chromosomeToCheck ptr to the mvc chromosome to check
 * @param maxDeriv the maximal difference that two values whithin a mvc can have
 */
template<typename VALUE_TYPE, typename FITNESS_TYPE>
inline void checkContinuity(typename BaseChromosome<FITNESS_TYPE>::ptr chromosomeToCheck, VALUE_TYPE maxDeriv)
{
    typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr mvcPtr = test_helper::convertBaseChromosome<VALUE_TYPE,
            FITNESS_TYPE>(chromosomeToCheck);

    VALUE_TYPE lastVal = *(mvcPtr->getContainer().begin());

    int i = 0;
    for(const auto& it : mvcPtr->getContainer())
    {

        VALUE_TYPE val = std::abs(lastVal - it);
        BOOST_CHECK(val <= maxDeriv); //TODO (bewo): Why this offset?s
        lastVal = it;
        i++;
    }
}

/**
 * Checks whether a given chromosome has a specific first value
 * @param chromosomeToCheck ptr to the mvc chromosome to check
 * @param start the start value
 * @param tolerance a tolerance that the first value can differ from
 */
template<typename VALUE_TYPE, typename FITNESS_TYPE>
inline void checkForStart(typename BaseChromosome<FITNESS_TYPE>::ptr chromosomeToCheck, VALUE_TYPE start, VALUE_TYPE tolerance = 0)
{
    typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr mvcPtr = test_helper::convertBaseChromosome<VALUE_TYPE,
                FITNESS_TYPE>(chromosomeToCheck);


    VALUE_TYPE firstVal = *(mvcPtr->getContainer().begin());
    BOOST_CHECK(std::abs(firstVal - start) <= tolerance);
}


/**
 * Check whether a given chromosome still has values within the specified bounds
 * @param chromosomeToCheck ptr to the mvc chromosome to check
 * @param lower the lower limit (min)
 * @param upper the upper limit (max)
 */
template<typename VALUE_TYPE, typename FITNESS_TYPE>
inline void checkLimits(typename BaseChromosome<FITNESS_TYPE>::ptr chromosomeToCheck, VALUE_TYPE lower,
        VALUE_TYPE upper)
{
    typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr mvcPtr = test_helper::convertBaseChromosome<VALUE_TYPE,
            FITNESS_TYPE>(chromosomeToCheck);

    for(const auto& it : mvcPtr->getContainer())
    {
        BOOST_CHECK(it >= lower);
        BOOST_CHECK(it <= upper);
    }
}

} /* namespace test_helper */

