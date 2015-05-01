#pragma once

#include <geneial/core/operations/crossover/SmoothedMultiValueChromosomeNPointCrossover.h>

namespace geneial
{
namespace operation
{
namespace crossover
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set SmoothedMultiValueChromosomeNPointCrossover<
        VALUE_TYPE, FITNESS_TYPE>::doCrossover(typename BaseChromosome<FITNESS_TYPE>::ptr mommy,
        typename BaseChromosome<FITNESS_TYPE>::ptr daddy)
{
    typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set result = MultiValueChromosomeNPointCrossover<
            VALUE_TYPE, FITNESS_TYPE>::doCrossover(mommy, daddy);

    for (const auto& chromosome : result){
    {
        Smoothing::restoreSmoothness<VALUE_TYPE, FITNESS_TYPE>(
                std::dynamic_pointer_cast < MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> > chromosome,
                _builderSettings.getEps(), _builderSettings.getRandomMin(), _builderSettings.getRandomMax());

    }

    return result;

}

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */
