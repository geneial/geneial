#pragma once

#include <geneial/core/operations/crossover/SmoothedMultiValueChromosomeNPointCrossover.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(crossover)
{
using ::geneial::utility::Smoothing;

geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set SmoothedMultiValueChromosomeNPointCrossover<
        VALUE_TYPE, FITNESS_TYPE>::doMultiValueCrossover(
        const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
        const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const

{
    //We can do this since this is subclass, however:
    //TODO(bewo) make this class a decorator for an arbitrary crossover function

    auto result = MultiValueChromosomeNPointCrossover<VALUE_TYPE, FITNESS_TYPE>::doMultiValueCrossover(mommy, daddy);

    const auto &builderSettings =
        (static_cast<const ContinousMultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>&>(
                this->getBuilderFactory().getSettings()));

    const auto eps = builderSettings.getEps();
    const auto min = builderSettings.getRandomMin();
    const auto max = builderSettings.getRandomMax();
    const auto hasStart = builderSettings.hasStart();
    const auto startValue = builderSettings.getStartValue();

    for (const auto& chromosome : result)
    {
        Smoothing::restoreSmoothness<VALUE_TYPE, FITNESS_TYPE>(
                std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(chromosome), eps, min, max, hasStart, startValue);
    }
    return result;

}

} /* geneial_export_namespace */
} /* private namespace crossover */
} /* private namespace operation */
} /* private namespace geneial */
