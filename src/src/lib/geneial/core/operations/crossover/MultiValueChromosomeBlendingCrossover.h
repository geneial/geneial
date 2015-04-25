#ifndef __GENEIAL_MULTIVALUE_CHROMOSOME_BLENDING_CROSSOVER_H_
#define __GENEIAL_MULTIVALUE_CHROMOSOME_BLENDING_CROSSOVER_H_

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
    MultiValueChromosomeBlendingCrossover(MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> *builderSettings,
            MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> *builderFactory, InterpolateBeta interpolationMethod,
            OffspringMode offspringMode, unsigned int numChilds) :
            _builderSettings(builderSettings), _builderFactory(builderFactory), _interpolationMethod(
                    interpolationMethod), _offspringMode(offspringMode), _numChilds(numChilds)

    {
        assert(_builderSettings != NULL);
        assert(_builderFactory != NULL);
        assert(_numChilds >= 1);
    }

    virtual ~MultiValueChromosomeBlendingCrossover()
    {
    }

    virtual bool inline isSymmetric() const
    {
        return false;
    }

    virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set
    doCrossover(typename BaseChromosome<FITNESS_TYPE>::ptr mommy, typename BaseChromosome<FITNESS_TYPE>::ptr daddy);

    MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>* const & getBuilderSettings() const
    {
        return _builderSettings;
    }

    MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>* const & getBuilderFactory() const
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
    MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> * _builderSettings;
    MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> *_builderFactory;
    InterpolateBeta _interpolationMethod;
    OffspringMode _offspringMode;
    unsigned int _numChilds;
};

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/crossover/MultiValueChromosomeBlendingCrossover.hpp>

#endif /* __GENEIAL_MULTIVALUE_CHROMOSOME_BLENDING_CROSSOVER_H_ */
