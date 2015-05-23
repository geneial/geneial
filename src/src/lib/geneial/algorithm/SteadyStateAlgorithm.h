#pragma once

#include <geneial/algorithm/BaseGeneticAlgorithm.h>

#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>
#include <geneial/core/operations/selection/RouletteWheelSelection.h>
#include <geneial/core/operations/coupling/RandomCouplingOperation.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/core/operations/replacement/ReplaceWorstOperation.h>
#include <geneial/core/operations/choosing/ChooseRandom.h>
#include <geneial/core/operations/mutation/UniformMutationOperation.h>

namespace geneial
{
namespace algorithm
{

template<typename FITNESS_TYPE>
class SteadyStateAlgorithm: public BaseGeneticAlgorithm<FITNESS_TYPE>
{
protected:
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

public:
    class Builder;

    virtual void solve() override;

    virtual ~SteadyStateAlgorithm()
    {
    }
};

template<typename FITNESS_TYPE>
class SteadyStateAlgorithm<FITNESS_TYPE>::Builder : public BaseGeneticAlgorithm<FITNESS_TYPE>::Builder{

public:
    virtual std::shared_ptr<stopping_criteria::BaseStoppingCriterion<FITNESS_TYPE>> getDefaultStoppingCriterion() const;

    virtual std::shared_ptr<selection::BaseSelectionOperation<FITNESS_TYPE>> getDefaultSelectionOperation() const;

    virtual std::shared_ptr<coupling::BaseCouplingOperation<FITNESS_TYPE>> getDefaultCouplingOperation() const;

    virtual std::shared_ptr<crossover::BaseCrossoverOperation<FITNESS_TYPE>> getDefaultCrossoverOperation() const;

    virtual std::shared_ptr<replacement::BaseReplacementOperation<FITNESS_TYPE>> getDefaultReplacementOperation() const;

    virtual std::shared_ptr<mutation::BaseMutationOperation<FITNESS_TYPE>> getDefaultMutationOperation() const;

    virtual std::unique_ptr<BaseGeneticAlgorithm<FITNESS_TYPE>> build() override;

};



}
}

#include <geneial/algorithm/SteadyStateAlgorithm.hpp>
#include <geneial/algorithm/SteadyStateAlgorithmBuilder.hpp>
