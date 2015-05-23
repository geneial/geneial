#pragma once

#include <geneial/core/operations/crossover/MultiValueChromosomeCrossoverOperation.h>

#include <cassert>
#include <geneial/utility/Interpolators.h>

namespace geneial
{
namespace operation
{
namespace crossover
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeBlendingCrossover: public MultiValueChromosomeCrossoverOperation<VALUE_TYPE,FITNESS_TYPE>

{
public:

    enum InterpolateBeta
    {
        INTERPOLATE_RANDOM = 1, INTERPOLATE_LINEARLY, INTERPOLATE_COSINE, INTERPOLATE_NONE
    };

    enum OffspringMode
    {
        FIXED_AMOUNT, RANDOM_AMOUNT
    };

    //TODO (bewo): are settings necessary?
    MultiValueChromosomeBlendingCrossover(
            const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory,
            const InterpolateBeta interpolationMethod,
            const OffspringMode offspringMode, const unsigned int numChilds) :
                MultiValueChromosomeCrossoverOperation<VALUE_TYPE,FITNESS_TYPE>(builderFactory),
                    _interpolationMethod(interpolationMethod), _offspringMode(offspringMode), _numChilds(numChilds)

    {
        assert(_numChilds >= 1);
    }

    virtual ~MultiValueChromosomeBlendingCrossover()
    {
    }

    virtual bool inline isSymmetric() const override
    {
        return false;
    }

    virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set doMultiValueCrossover(
            const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
            const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const override;

    InterpolateBeta getInterpolationMethod() const
    {
        return _interpolationMethod;
    }

    unsigned int getNumChilds() const
    {
        return _numChilds;
    }

    OffspringMode getOffspringMode() const
    {
        return _offspringMode;
    }

private:
    const InterpolateBeta _interpolationMethod;
    const OffspringMode _offspringMode;
    unsigned int _numChilds;
};

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/crossover/MultiValueChromosomeBlendingCrossover.hpp>

