#pragma once

#include <geneial/core/operations/mutation/MultiValueChromosomeMutationOperation.h>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>
#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>
#include <geneial/utility/Smoothing.h>

#include <cassert>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(mutation)
{
using ::geneial::population::chromosome::ContinousMultiValueBuilderSettings;
using ::geneial::population::chromosome::ContinousMultiValueChromosomeFactory;

geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class SmoothPeakMutationOperation: public MultiValueChromosomeMutationOperation<VALUE_TYPE,FITNESS_TYPE>
{

private:
    unsigned int _maxLeftEps;

    unsigned int _maxRightEps;

    FITNESS_TYPE _maxElevation;

protected:
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
public:

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


    class Builder : MultiValueChromosomeMutationOperation<VALUE_TYPE,FITNESS_TYPE>::Builder
       {
       protected:
            unsigned int _maxLeftEps;

            unsigned int _maxRightEps;

            FITNESS_TYPE _maxElevation;
       public:

            const constexpr static unsigned int DEFAULT_MAX_LEFT_EPS = 2;

            const constexpr static unsigned int DEFAULT_MAX_RIGHT_EPS = 2;

            const constexpr static FITNESS_TYPE DEFAULT_MAX_ELEVATION = 5;

           Builder(const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory) :
                   MultiValueChromosomeMutationOperation<VALUE_TYPE, FITNESS_TYPE>::Builder(builderFactory),
                   _maxLeftEps(DEFAULT_MAX_LEFT_EPS),
                   _maxRightEps(DEFAULT_MAX_RIGHT_EPS),
                   _maxElevation(DEFAULT_MAX_ELEVATION)
           {
           }

           Builder(const std::shared_ptr<const MutationSettings> &settings,
                   const std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation,
                   const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory) :
                   MultiValueChromosomeMutationOperation<VALUE_TYPE, FITNESS_TYPE>::Builder(settings, choosingOperation,
                           builderFactory),
                           _maxLeftEps(DEFAULT_MAX_LEFT_EPS),
                           _maxRightEps(DEFAULT_MAX_RIGHT_EPS),
                           _maxElevation(DEFAULT_MAX_ELEVATION)
           {
           }

           void setMaxElevation(FITNESS_TYPE maxElevation)
           {
               _maxElevation = maxElevation;
           }

           void setMaxLeftEps(unsigned int maxLeftEps)
           {
               _maxLeftEps = maxLeftEps;
           }

           void setMaxRightEps(unsigned int maxRightEps)
           {
               _maxRightEps = maxRightEps;
           }

           virtual typename BaseMutationOperation<FITNESS_TYPE>::ptr create() override
           {
               typename BaseMutationOperation<FITNESS_TYPE>::ptr protoype(
                       new SmoothPeakMutationOperation<VALUE_TYPE, FITNESS_TYPE>(
                               this->_maxLeftEps,
                               this->_maxRightEps,
                               this->_maxElevation,
                               this->_settings,
                               this->_choosingOperation,
                               this->_builderFactory));
               return std::move(protoype);
           }

       };



};
//class

} /* geneial_export_namespace */
} /* private namespace mutation */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/mutation/SmoothPeakMutationOperation.hpp>

