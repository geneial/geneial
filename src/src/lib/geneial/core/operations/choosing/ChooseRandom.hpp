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
        typename Population<FITNESS_TYPE>::chromosome_container chromosomeInputSet)
{

    typename Population<FITNESS_TYPE>::chromosome_container::iterator chromosomeInputContainer_it;
    typename Population<FITNESS_TYPE>::chromosome_container chromosomeOutputSet;

    for (chromosomeInputContainer_it = chromosomeInputSet.begin();
            chromosomeInputContainer_it != chromosomeInputSet.end(); ++chromosomeInputContainer_it)
    {
        //Uses Mutation Settings
        if (Random::instance()->decision(this->getSettings()->getPropability()))
        {
            chromosomeOutputSet.push_back(*chromosomeInputContainer_it);
        }
    }
    return chromosomeOutputSet;
}

} /* namespace choosing */
} /* namespace operation */
} /* namespace geneial */

