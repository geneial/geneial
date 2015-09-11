#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeCrossoverOperation.h>
#include <geneial/utility/mixins/EnableMakeShared.h>

#include <cassert>
#include <geneial/utility/Interpolators.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(crossover)
{
using ::geneial::population::Population;
using ::geneial::utility::Interpolators;
using ::geneial::utility::EnableMakeShared;

geneial_export_namespace
{


template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeBlendingCrossover: public MultiValueChromosomeCrossoverOperation<VALUE_TYPE,FITNESS_TYPE>,
                                             public virtual EnableMakeShared<MultiValueChromosomeBlendingCrossover<VALUE_TYPE,FITNESS_TYPE>>
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

    virtual bool isSymmetric() const override
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

    class Builder : public MultiValueChromosomeCrossoverOperation<VALUE_TYPE,FITNESS_TYPE>::Builder
    {
    protected:

        InterpolateBeta _interpolationMethod;

        OffspringMode _offspringMode;

        unsigned int _numChilds;

    public:

        const static InterpolateBeta DEFAULT_INTERPOLATION_METHOD = INTERPOLATE_RANDOM;

        const static OffspringMode DEFAULT_OFFSPRING_MODE = RANDOM_AMOUNT;

        const static unsigned int DEFAULT_NUM_CHILD = 1;

        Builder(const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> & builderFactory) :
                MultiValueChromosomeCrossoverOperation<VALUE_TYPE, FITNESS_TYPE>::Builder(builderFactory),
                _interpolationMethod(DEFAULT_INTERPOLATION_METHOD),
                _offspringMode(DEFAULT_OFFSPRING_MODE),
                _numChilds(DEFAULT_NUM_CHILD)
        {
        }


        virtual typename BaseCrossoverOperation<FITNESS_TYPE>::ptr create() override
        {

            if(! this->_builderFactory )
            {
                throw new std::runtime_error("Must set a Chromosome Factory to build MultiValueCrossover");
            }

            //Ctor already takes care that values are initialized to proper values..
            return MultiValueChromosomeBlendingCrossover<VALUE_TYPE,FITNESS_TYPE>
                        ::makeShared(
                                this->_builderFactory,
                                _interpolationMethod,
                                _offspringMode,
                                _numChilds
                                );
        }

        InterpolateBeta getInterpolationMethod() const
        {
            return _interpolationMethod;
        }

        Builder& setInterpolationMethod(InterpolateBeta interpolationMethod)
        {
            _interpolationMethod = interpolationMethod;
            return *this;
        }

        unsigned int getNumChilds() const
        {
            return _numChilds;
        }

        Builder& setNumChilds(unsigned int numChilds)
        {
            _numChilds = numChilds;
            return *this;
        }

        OffspringMode getOffspringMode() const
        {
            return _offspringMode;
        }

        Builder& setOffspringMode(OffspringMode offspringMode)
        {
            _offspringMode = offspringMode;
            return *this;
        }
    };


private:
    const InterpolateBeta _interpolationMethod;

    const OffspringMode _offspringMode;

    unsigned int _numChilds;
};

} /* geneial_export_namespace */
} /* private namespace crossover */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/crossover/MultiValueChromosomeBlendingCrossover.hpp>

