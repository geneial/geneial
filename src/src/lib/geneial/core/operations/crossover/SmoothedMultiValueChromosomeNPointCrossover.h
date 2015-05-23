#pragma once

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>
#include <geneial/utility/Smoothing.h>

#include <cassert>

namespace geneial
{
namespace operation
{
namespace crossover
{

using geneial::utility::Random;
using geneial::population::chromosome::MultiValueChromosome;

//TODO (bewo) allow random crossover width per settings

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class SmoothedMultiValueChromosomeNPointCrossover: public MultiValueChromosomeNPointCrossover<VALUE_TYPE, FITNESS_TYPE>
{

public:
    SmoothedMultiValueChromosomeNPointCrossover(
            const std::shared_ptr<const MultiValueChromosomeNPointCrossoverSettings> &crossoverSettings,
            const std::shared_ptr<ContinousMultiValueChromosomeFactory<VALUE_TYPE,FITNESS_TYPE>> &builderFactory
            ) :
            MultiValueChromosomeNPointCrossover<VALUE_TYPE, FITNESS_TYPE>(crossoverSettings, builderFactory)
    {
    }

    virtual ~SmoothedMultiValueChromosomeNPointCrossover()
    {
    }

    virtual bool inline isSymmetric() const override
    {
        return false;
    }

    virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set doMultiValueCrossover(
            const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
            const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const override;
};

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/crossover/SmoothedMultiValueChromosomeNPointCrossover.hpp>

