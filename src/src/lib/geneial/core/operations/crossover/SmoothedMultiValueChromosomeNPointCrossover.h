#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>
#include <geneial/utility/mixins/EnableMakeShared.h>
#include <geneial/utility/Smoothing.h>

#include <cassert>


geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(crossover)
{
using ::geneial::population::Population;
using ::geneial::population::chromosome::MultiValueChromosome;
using ::geneial::operation::coupling::BaseCouplingOperation;
using ::geneial::utility::Random;
using ::geneial::population::chromosome::ContinousMultiValueBuilderSettings;
using ::geneial::population::chromosome::ContinousMultiValueChromosomeFactory;
using ::geneial::utility::EnableMakeShared;


geneial_export_namespace
{

//TODO (bewo) allow random crossover width per settings

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class SmoothedMultiValueChromosomeNPointCrossover:  public MultiValueChromosomeNPointCrossover<VALUE_TYPE, FITNESS_TYPE>,
                                                    public virtual EnableMakeShared<SmoothedMultiValueChromosomeNPointCrossover<VALUE_TYPE,FITNESS_TYPE>>

{
protected:
    SmoothedMultiValueChromosomeNPointCrossover(
                const std::shared_ptr<const MultiValueChromosomeNPointCrossoverSettings> &crossoverSettings,
                const std::shared_ptr<ContinousMultiValueChromosomeFactory<VALUE_TYPE,FITNESS_TYPE>> &builderFactory
                ) :
                MultiValueChromosomeNPointCrossover<VALUE_TYPE, FITNESS_TYPE>(crossoverSettings, builderFactory)
        {
        }
public:
    //Avoid DoD problem by specifying the required make shared
    using EnableMakeShared<SmoothedMultiValueChromosomeNPointCrossover<VALUE_TYPE,FITNESS_TYPE>>::makeShared;
    virtual ~SmoothedMultiValueChromosomeNPointCrossover()
    {
    }

    virtual bool isSymmetric() const override
    {
        return false;
    }

    virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set doMultiValueCrossover(
            const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
            const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const override;


    class Builder : public MultiValueChromosomeNPointCrossover<VALUE_TYPE,FITNESS_TYPE>::Builder
    {
    public:

        Builder(const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> & builderFactory) :
                MultiValueChromosomeNPointCrossover<VALUE_TYPE, FITNESS_TYPE>::Builder(builderFactory)
        {
        }

        virtual typename BaseCrossoverOperation<FITNESS_TYPE>::ptr create() override
        {
            if(! this->_builderFactory )
            {
                throw new std::runtime_error("Must set a Chromosome Factory to build MultiValueCrossover");
            }

            return std::move(
                    SmoothedMultiValueChromosomeNPointCrossover::makeShared(
                                        this->_crossoverSettings,
                                        std::dynamic_pointer_cast<ContinousMultiValueChromosomeFactory<VALUE_TYPE,FITNESS_TYPE>>
                                                                                                                (this->_builderFactory)
                                      )
            );
        }
    };
};

} /* geneial_export_namespace */
} /* private namespace crossover */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/crossover/SmoothedMultiValueChromosomeNPointCrossover.hpp>

