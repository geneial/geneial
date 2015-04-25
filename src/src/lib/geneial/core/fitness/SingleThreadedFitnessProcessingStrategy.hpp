#ifndef __GENEIAL_SINGLE_THREADED_FITNESS_PROCESSING_STRATEGY_HPP_
#define __GENEIAL_SINGLE_THREADED_FITNESS_PROCESSING_STRATEGY_HPP_

#include <geneial/core/fitness/SingleThreadedFitnessProcessingStrategy.h>

namespace geneial
{

template<typename FITNESS_TYPE>
void SingleThreadedFitnessProcessingStrategy<FITNESS_TYPE>::ensureHasFitness(
        const typename Population<FITNESS_TYPE>::chromosome_container &refcontainer)
{
    for (typename Population<FITNESS_TYPE>::chromosome_container::const_iterator it = refcontainer.begin();
            it != refcontainer.end(); ++it)
    {
        (*it)->getFitness(); //This will trigger lazy evaluation.
    }
}

} /* namespace geneial */

#endif /* __GENEIAL_SINGLE_THREADED_FITNESS_PROCESSING_STRATEGY_HPP_ */
