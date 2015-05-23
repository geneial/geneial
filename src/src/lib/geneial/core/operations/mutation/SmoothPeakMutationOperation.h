#pragma once

#include <geneial/core/operations/mutation/MultiValueChromosomeMutationOperation.h>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>
#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>
#include <geneial/utility/Smoothing.h>

#include <cassert>

namespace geneial
{
namespace operation
{
namespace mutation
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class SmoothPeakMutationOperation: public MultiValueChromosomeMutationOperation<VALUE_TYPE,FITNESS_TYPE>
{

private:
    unsigned int _maxLeftEps;
    unsigned int _maxRightEps;
    FITNESS_TYPE _maxElevation;

public:
    SmoothPeakMutationOperation(
            unsigned int maxLeftEps,
            unsigned int maxRightEps,
            FITNESS_TYPE maxElevation,
            const std::shared_ptr<const MutationSettings> &settings,
            const std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation,
            const std::shared_ptr<ContinousMultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory) :

                MultiValueChromosomeMutationOperation<VALUE_TYPE,FITNESS_TYPE>(
                    settings,
                    choosingOperation,
                    builderFactory
                    ),

            _maxLeftEps(maxLeftEps),
            _maxRightEps(maxRightEps),
            _maxElevation(maxElevation)
    {
        //TODO (bewo): Make some assertions regarding eps and builder here
    }

    virtual ~SmoothPeakMutationOperation()
    {
    }

    /*
     *  Returns a new chromosome which is a partially mutated version of the old one.
     *  */
    virtual typename Population<FITNESS_TYPE>::chromosome_container doMutate(
            const typename Population<FITNESS_TYPE>::chromosome_container &mutants, BaseManager<FITNESS_TYPE> &manager) const override;

    FITNESS_TYPE getMaxElevation() const
    {
        return _maxElevation;
    }

    unsigned int getMaxLeftEps() const
    {
        return _maxLeftEps;
    }

    unsigned int getMaxRightEps() const
    {
        return _maxRightEps;
    }
};
//class

} /* namespace mutation */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/mutation/SmoothPeakMutationOperation.hpp>

