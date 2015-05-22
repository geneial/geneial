#pragma once

#include <geneial/core/operations/crossover/BaseCrossoverOperation.h>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>


#include <cassert>
#include <memory>

namespace geneial
{
namespace operation
{
namespace crossover
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeCrossoverOperation: public BaseCrossoverOperation<FITNESS_TYPE>
{
private:

    std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> _builderFactory;

protected:

    virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set doMultiValueCrossover(
            const typename MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
            const typename MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const = 0;


    virtual typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr createChildCandidate(
            typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior howToPopulate = BaseChromosomeFactory<
                    FITNESS_TYPE>::LET_UNPOPULATED) const
    {
        const auto child_candidate = std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>>(
                this->getBuilderFactory().createChromosome(howToPopulate));
        assert(child_candidate);
        return child_candidate;
    }

public:
    MultiValueChromosomeCrossoverOperation(
            const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory
            ) : BaseCrossoverOperation<FITNESS_TYPE>(), _builderFactory(builderFactory)
    {
    }

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

};

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/crossover/MultiValueChromosomeAverageCrossover.hpp>

