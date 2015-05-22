#pragma once

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/core/population/builder/ContinousMultiIntValueChromosomeFactory.h>
#include <geneial/utility/Smoothing.h>
#include <cassert>

namespace geneial
{
namespace operation
{
namespace crossover
{

using namespace geneial::utility;
using namespace geneial::population::chromosome;

//TODO (bewo) allow random crossover width per settings

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class SmoothedMultiValueChromosomeNPointCrossover: public MultiValueChromosomeNPointCrossover<VALUE_TYPE, FITNESS_TYPE>
{
public:
    SmoothedMultiValueChromosomeNPointCrossover(
            const std::shared_ptr<const MultiValueChromosomeNPointCrossoverSettings> &crossoverSettings,
            const std::shared_ptr<const ContinousMultiIntValueChromosomeFactory<FITNESS_TYPE>> &builderFactory
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
            const typename MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
            const typename MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const override;
};

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/crossover/SmoothedMultiValueChromosomeNPointCrossover.hpp>

