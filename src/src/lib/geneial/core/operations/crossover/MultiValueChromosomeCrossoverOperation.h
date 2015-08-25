#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/crossover/BaseCrossoverOperation.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>


#include <cassert>
#include <memory>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(crossover)
{
using ::geneial::population::Population;
using ::geneial::population::chromosome::MultiValueChromosome;
using ::geneial::population::chromosome::BaseChromosomeFactory;
using ::geneial::population::chromosome::MultiValueChromosomeFactory;
using ::geneial::operation::coupling::BaseCouplingOperation;


geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeCrossoverOperation: public BaseCrossoverOperation<FITNESS_TYPE>
{
private:

    std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> _builderFactory;

protected:

    virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set doMultiValueCrossover(
            const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
            const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const = 0;


    virtual typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr createChildCandidate(
            typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior howToPopulate = BaseChromosomeFactory<
                    FITNESS_TYPE>::LET_UNPOPULATED) const
    {
        const auto child_candidate = std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>>(
                this->getBuilderFactory().createChromosome(howToPopulate));
        assert(child_candidate);
        return child_candidate;
    }

    MultiValueChromosomeCrossoverOperation(
            const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory
            ) : BaseCrossoverOperation<FITNESS_TYPE>(), _builderFactory(builderFactory)
    {
    }

public:

    virtual ~MultiValueChromosomeCrossoverOperation()
    {
    }

    virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set doCrossover(
            const typename BaseChromosome<FITNESS_TYPE>::const_ptr &mommy,
            const typename BaseChromosome<FITNESS_TYPE>::const_ptr &daddy) const override
    {
        const auto mvc_mommy = std::dynamic_pointer_cast<const MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(mommy);
        assert(mvc_mommy);

        const auto mvc_daddy = std::dynamic_pointer_cast<const MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(daddy);
        assert(mvc_daddy);

        return this->doMultiValueCrossover(mvc_mommy,mvc_daddy);
    }


    inline MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> & getBuilderFactory() const
    {
        return *_builderFactory;
    }

    void setBuilderFactory(
            const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> & builderFactory)
    {
        _builderFactory = builderFactory;
    }

    class Builder: public BaseCrossoverOperation<FITNESS_TYPE>::Builder
    {
        public:
            Builder(const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> & builderFactory):
                _builderFactory(builderFactory)
            {
            }

            Builder& setBuilderFactory(
                    const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> & builderFactory)
            {
                _builderFactory = builderFactory;
                return *this;
            }

        protected:
            std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> _builderFactory;
    };

};

} /* geneial_export_namespace */
} /* private namespace crossover */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/crossover/MultiValueChromosomeAverageCrossover.hpp>

