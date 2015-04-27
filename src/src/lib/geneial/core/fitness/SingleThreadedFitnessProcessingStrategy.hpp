#pragma once

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

