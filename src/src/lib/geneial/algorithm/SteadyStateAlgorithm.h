#pragma once

#include <geneial/algorithm/BaseGeneticAlgorithm.h>

namespace geneial
{
namespace algorithm
{

template<typename FITNESS_TYPE>
class SteadyStateAlgorithm: public BaseGeneticAlgorithm<FITNESS_TYPE>
{
public:

    SteadyStateAlgorithm(stopping_criteria::BaseStoppingCriterion<FITNESS_TYPE> &stoppingCriterion,
            selection::BaseSelectionOperation<FITNESS_TYPE> &selectionOperation,
            coupling::BaseCouplingOperation<FITNESS_TYPE> &couplingOperation,
            crossover::BaseCrossoverOperation<FITNESS_TYPE> &crossoverOperation,
            replacement::BaseReplacementOperation<FITNESS_TYPE> &replacementOperation,
            mutation::BaseMutationOperation<FITNESS_TYPE> &mutationOperation,
            BaseChromosomeFactory<FITNESS_TYPE> &chromosomeFactory) :
            BaseGeneticAlgorithm<FITNESS_TYPE>(stoppingCriterion, selectionOperation, couplingOperation,
                    crossoverOperation, replacementOperation, mutationOperation, chromosomeFactory)
    {
    }

    virtual void solve() override;

    virtual ~SteadyStateAlgorithm()
    {
    }
};

}
}

#include <geneial/algorithm/SteadyStateAlgorithm.hpp>
