#pragma once

#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/population/builder/ContinousMultiIntValueChromosomeFactory.h>
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
class SmoothPeakMutationOperation: public BaseMutationOperation<FITNESS_TYPE>
{

private:
    const ContinousMultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &_builderSettings;
    const ContinousMultiIntValueChromosomeFactory<FITNESS_TYPE> &_builderFactory;

    unsigned int _maxLeftEps;
    unsigned int _maxRightEps;
    FITNESS_TYPE _maxElevation;

public:
    /*
     * UniformMutationOperation Mutates a chromosome, by replacing some of it's values randomly.
     */
    SmoothPeakMutationOperation(
            const MutationSettings &settings,
            const BaseChoosingOperation<FITNESS_TYPE> &choosingOperation,
            const ContinousMultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &builderSettings,
            const ContinousMultiIntValueChromosomeFactory<FITNESS_TYPE> &builderFactory,
            unsigned int maxLeftEps,
            unsigned int maxRightEps, FITNESS_TYPE maxElevation) :
            BaseMutationOperation<FITNESS_TYPE>(settings, choosingOperation), _builderSettings(builderSettings), _builderFactory(
                    builderFactory), _maxLeftEps(maxLeftEps), _maxRightEps(maxRightEps), _maxElevation(maxElevation)
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
            const typename Population<FITNESS_TYPE>::chromosome_container &mutants, BaseManager<FITNESS_TYPE> &manager) override;

    ContinousMultiIntValueChromosomeFactory<FITNESS_TYPE> & getBuilderFactory() const
    {
        return _builderFactory;
    }

    ContinousMultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> & getBuilderSettings() const
    {
        return _builderSettings;
    }

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

