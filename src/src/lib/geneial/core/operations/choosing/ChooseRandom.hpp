#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/choosing/ChooseRandom.h>
#include <geneial/core/population/Population.h>
#include <geneial/utility/Random.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(choosing)
{
using ::geneial::population::Population;
using ::geneial::utility::Random;

geneial_export_namespace
{
template<typename FITNESS_TYPE>
typename Population<FITNESS_TYPE>::chromosome_container ChooseRandom<FITNESS_TYPE>::doChoose(
        const typename Population<FITNESS_TYPE>::chromosome_container &chromosomeInputSet) const
{

    typename Population<FITNESS_TYPE>::chromosome_container chromosomeOutputSet;

    //Reserve upper limit i.e. the input size, so we avoid resizing later.
    chromosomeOutputSet.reserve(chromosomeInputSet.size());

    //Iterate over all chromosomes in set and decide whether to keep it or not.
    for (const auto &chromosome : chromosomeInputSet)
    {
        if (Random::decision(this->getProbability()))
        {
            chromosomeOutputSet.emplace_back(chromosome);
        }
    }
    return chromosomeOutputSet;
}

} /* geneial_export_namespace */
} /* private namespace choosing */
} /* private namespace operation */
} /* private namespace geneial */

