#pragma once

#include <geneial/core/operations/crossover/BaseCrossoverOperation.h>
#include <geneial/core/operations/coupling/BaseCouplingOperation.h>

#include <cassert>
#include <geneial/utility/Interpolators.h>

namespace geneial
{
namespace operation
{
namespace crossover
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeBlendingCrossover: public BaseCrossoverOperation<FITNESS_TYPE>
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
    MultiValueChromosomeBlendingCrossover(const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &builderSettings,
            MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> &builderFactory, const InterpolateBeta interpolationMethod,
            const OffspringMode offspringMode, const unsigned int numChilds) :
            _builderSettings(builderSettings), _builderFactory(builderFactory), _interpolationMethod(
                    interpolationMethod), _offspringMode(offspringMode), _numChilds(numChilds)

    {
        assert(_numChilds >= 1);
    }

    virtual ~MultiValueChromosomeBlendingCrossover()
    {
    }

    virtual bool inline isSymmetric() override const
    {
        return false;
    }

    virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set
    doCrossover(typename BaseChromosome<FITNESS_TYPE>::ptr mommy, typename BaseChromosome<FITNESS_TYPE>::ptr daddy) const override;

    MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> const & getBuilderSettings() const
    {
        return _builderSettings;
    }

    MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>  & getBuilderFactory() const
    {
        return _builderFactory;
    }

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
    const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> & _builderSettings;
    MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> &_builderFactory;
    const InterpolateBeta _interpolationMethod;
    const OffspringMode _offspringMode;
    unsigned int _numChilds;
};

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/crossover/MultiValueChromosomeBlendingCrossover.hpp>

