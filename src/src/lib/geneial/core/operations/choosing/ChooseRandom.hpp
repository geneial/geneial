#pragma once

#include <geneial/core/operations/choosing/ChooseRandom.h>
#include <geneial/core/population/Population.h>
#include <geneial/utility/Random.h>

namespace geneial
{
namespace operation
{
namespace choosing
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename Population<FITNESS_TYPE>::chromosome_container ChooseRandom<VALUE_TYPE, FITNESS_TYPE>::doChoose(
        const typename Population<FITNESS_TYPE>::chromosome_container &chromosomeInputSet) const
{

    typename Population<FITNESS_TYPE>::chromosome_container chromosomeOutputSet;

    //Reserve upper limit i.e. the input size, so we avoid resizing later.
    chromosomeOutputSet.reserve(chromosomeInputSet.size());

    //Iterate over all chromosomes in set and decide whether to keep it or not.
    for (const auto &chromosome : chromosomeInputSet)
    {
        if (Random::decision(this->getSettings().getPropability()))
        {
            chromosomeOutputSet.push_back(chromosome);
        }
    }
    return chromosomeOutputSet;
}

} /* namespace choosing */
} /* namespace operation */
} /* namespace geneial */

