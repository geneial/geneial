#pragma once

#include <geneial/core/operations/crossover/MultiValueChromosomeBlendingCrossover.h>
#include <geneial/core/operations/crossover/BaseCrossoverOperation.h>
#include <geneial/utility/Random.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(crossover)
{
using ::geneial::population::Population;
using ::geneial::population::chromosome::MultiValueChromosome;
using ::geneial::operation::coupling::BaseCouplingOperation;

geneial_export_namespace
{
using namespace geneial::utility;

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set MultiValueChromosomeBlendingCrossover<VALUE_TYPE,
        FITNESS_TYPE>::doMultiValueCrossover(
        const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
        const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const
{
    typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set resultset;

    //Determine the amount of chromosomes to generate
    unsigned int chromosomesToGenerate;

    if (_offspringMode == FIXED_AMOUNT)
    {
        chromosomesToGenerate = _numChilds;
    }
    else
    {
        chromosomesToGenerate = Random::generate<int>(1, _numChilds);
    }

    for (unsigned int i = 0; i < chromosomesToGenerate; i++)
    {
        InterpolateBeta interpolationMethod = _interpolationMethod;

        if (_interpolationMethod == INTERPOLATE_RANDOM)
        {
            interpolationMethod = (InterpolateBeta) (INTERPOLATE_RANDOM + Random::generate(1, 3));
        }

        const double beta1 = Random::generate<double>(0.0, 1.0);
        const double beta2 = Random::generate<double>(0.0, 1.0);

        auto child_candidate = this->createChildCandidate();

        assert(child_candidate);

        const unsigned int containerSize = this->getBuilderFactory().getSettings().getNum();

        const auto &daddy_container = mommy->getContainer();
        const auto &mommy_container = daddy->getContainer();
        auto &child_container = child_candidate->getContainer();

        auto mommy_it = mommy_container.cbegin();
        auto daddy_it = daddy_container.cbegin();

        for (unsigned int i = 0; mommy_it != mommy_container.end(); ++i)
        {
            double targetBeta;
            switch (interpolationMethod)
            {

                case INTERPOLATE_COSINE:
                {
                    targetBeta = Interpolators::cosineInterpolate<double>(beta1, beta2,
                            static_cast<double>(i) / (static_cast<double>(containerSize)));
                    break;
                }

                case INTERPOLATE_LINEARLY:
                {
                    targetBeta = Interpolators::linearInterpolate<double>(beta1, beta2,
                            static_cast<double>(i) / (static_cast<double>(containerSize)));
                    break;
                }

                default: //FALLTRHOUGH INTENDED
                case INTERPOLATE_NONE:
                {
                    targetBeta = beta1;
                    break;
                }

            }

            assert(targetBeta <= 1.0);
            assert(targetBeta >= 0);

            child_container[i] = ((*mommy_it * (1 - targetBeta) + *daddy_it * (targetBeta)));

            ++mommy_it;
            ++daddy_it;
        }

        //TODO(bewo): Restore Smoothness?

        resultset.emplace_back(child_candidate);
    }

    return std::move(resultset);
}

} /* geneial_export_namespace */
} /* private namespace crossover */
} /* private namespace operation */
} /* private namespace geneial */

