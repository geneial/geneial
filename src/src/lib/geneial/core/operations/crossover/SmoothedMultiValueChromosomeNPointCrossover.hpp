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
        VALUE_TYPE, FITNESS_TYPE>::doMultiValueCrossover(
        const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
        const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const

{
    //We can do this since this is subclass, however:
    //TODO(bewo) make this class a decorator for an arbitrary crossover function
    typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set result = MultiValueChromosomeNPointCrossover<
            VALUE_TYPE, FITNESS_TYPE>::doCrossover(mommy, daddy);

    for (const auto& chromosome : result)
    {
        {
            //We know that settings are ContinousMultiValueBuilderSettings due to continous setting constructor, so safe static upcasting
            const auto eps =
                    (static_cast<const ContinousMultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>&>(this->getBuilderSettings())).getEps();

            Smoothing::restoreSmoothness<VALUE_TYPE, FITNESS_TYPE>(
                    std::dynamic_pointer_cast < MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> > chromosome, eps,
                    this->getBuilderSettings().getRandomMin(), this->getBuilderSettings().getRandomMax());

        }

        return result;

    }

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */
