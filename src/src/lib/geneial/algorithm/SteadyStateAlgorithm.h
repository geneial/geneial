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

    SteadyStateAlgorithm(
            std::shared_ptr<stopping_criteria::BaseStoppingCriterion<FITNESS_TYPE>> stoppingCriterion,
            std::shared_ptr<selection::BaseSelectionOperation<FITNESS_TYPE>> selectionOperation,
            std::shared_ptr<coupling::BaseCouplingOperation<FITNESS_TYPE>> couplingOperation,
            std::shared_ptr<crossover::BaseCrossoverOperation<FITNESS_TYPE>> crossoverOperation,
            std::shared_ptr<replacement::BaseReplacementOperation<FITNESS_TYPE>> replacementOperation,
            std::shared_ptr<mutation::BaseMutationOperation<FITNESS_TYPE>> mutationOperation,
            std::shared_ptr<BaseChromosomeFactory<FITNESS_TYPE>> chromosomeFactory) :
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
