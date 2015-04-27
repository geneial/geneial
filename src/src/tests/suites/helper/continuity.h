#pragma once

#include <geneial/core/population/chromosome/MultiValueChromosome.h>

using namespace geneial::population::chromosome;

namespace test_helper
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
inline void checkContinuity(typename BaseChromosome<FITNESS_TYPE>::ptr chromosomeToCheck, VALUE_TYPE maxDeriv)
{
    typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr mvcPtr = test_helper::convertBaseChromosome<VALUE_TYPE,
            FITNESS_TYPE>(chromosomeToCheck);

    VALUE_TYPE lastVal = *(mvcPtr->getContainer().begin());
    for (typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_it it = mvcPtr->getContainer().begin();
            it != mvcPtr->getContainer().end(); ++it)
    {
        VALUE_TYPE val = std::abs(lastVal - *it);
        BOOST_CHECK(val <= maxDeriv);
        lastVal = *it;
    }
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
inline void checkLimits(typename BaseChromosome<FITNESS_TYPE>::ptr chromosomeToCheck, VALUE_TYPE lower,
        VALUE_TYPE upper)
{
    typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr mvcPtr = test_helper::convertBaseChromosome<VALUE_TYPE,
            FITNESS_TYPE>(chromosomeToCheck);

    for (typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_it it = mvcPtr->getContainer().begin();
            it != mvcPtr->getContainer().end(); ++it)
    {
        BOOST_CHECK(*it >= lower);
        BOOST_CHECK(*it <= upper);
    }
}

} /* namespace test_helper */

