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

    virtual std::shared_ptr<stopping_criteria::BaseStoppingCriterion<FITNESS_TYPE>> getDefaultStoppingCriterion() const
    {
        std::shared_ptr<stopping_criteria::BaseStoppingCriterion<FITNESS_TYPE>> stoppingCriterion(
                new stopping_criteria::MaxGenerationCriterion<FITNESS_TYPE>(100000));
        return stoppingCriterion;
    }

    virtual std::shared_ptr<selection::BaseSelectionOperation<FITNESS_TYPE>> getDefaultSelectionOperation() const
    {
        std::shared_ptr<selection::BaseSelectionOperation<FITNESS_TYPE>> selectionOperation(
                new selection::RouletteWheelSelection<FITNESS_TYPE>(std::shared_ptr<selection::SelectionSettings>(new selection::SelectionSettings(5))));
        return selectionOperation;
    }

    virtual std::shared_ptr<coupling::BaseCouplingOperation<FITNESS_TYPE>> getDefaultCouplingOperation() const
    {

        std::shared_ptr<coupling::CouplingSettings> couplingSettings(new coupling::CouplingSettings(20));

        std::shared_ptr<coupling::BaseCouplingOperation<FITNESS_TYPE>> couplingOperation(
                new coupling::RandomCouplingOperation<FITNESS_TYPE>(couplingSettings));

        return couplingOperation;
    }

    virtual std::shared_ptr<crossover::BaseCrossoverOperation<FITNESS_TYPE>> getDefaultCrossoverOperation() const
    {

        //In this case we need to check whether the chosen chromomsome builder is compatible with our default operation:
        auto mvcChromosomeFactory =
                std::dynamic_pointer_cast<crossover::MultiValueChromosomeFactory<int, FITNESS_TYPE>>(
                        *this->_chromosomeFactory);

        if (!mvcChromosomeFactory)
        {
            throw new std::runtime_error("Incompatible Chromosome Factory and default crossover strategy");
        }

        auto crossoverSettings = std::shared_ptr < crossover::MultiValueChromosomeNPointCrossoverSettings
                > (new crossover::MultiValueChromosomeNPointCrossoverSettings(1,
                        crossover::MultiValueChromosomeNPointCrossoverSettings::RANDOM_WIDTH, 1));

        std::shared_ptr<crossover::BaseCrossoverOperation<FITNESS_TYPE>> crossoverOperation(
                new crossover::MultiValueChromosomeNPointCrossover<int, FITNESS_TYPE>(crossoverSettings,
                        mvcChromosomeFactory));

        return crossoverOperation;
    }

    virtual std::shared_ptr<replacement::BaseReplacementOperation<FITNESS_TYPE>> getDefaultReplacementOperation() const
    {
        //TODO (bewo) derive to replace from Population Settings?!

        std::shared_ptr<replacement::BaseReplacementSettings>replacementSettings (new replacement::BaseReplacementSettings(replacement::BaseReplacementSettings::REPLACE_ALL_OFFSPRING, 20, 0));

        std::shared_ptr<replacement::ReplaceWorstOperation<FITNESS_TYPE>> replacementOperation(new replacement::ReplaceWorstOperation<FITNESS_TYPE>(replacementSettings));

        return replacementOperation;
    }

    virtual std::shared_ptr<mutation::BaseMutationOperation<FITNESS_TYPE>> getDefaultMutationOperation() const
    {

        std::shared_ptr<mutation::MutationSettings> mutationSettings(new mutation::MutationSettings(0.8, 0.0, 1));

        std::shared_ptr<choosing::ChooseRandom<int, FITNESS_TYPE>> mutationChoosingOperation( new choosing::ChooseRandom<int, FITNESS_TYPE>(mutationSettings));

        //In this case we need to check whether the chosen chromomsome builder is compatible with our default operation:
        auto mvcChromosomeFactory = std::dynamic_pointer_cast<MultiValueChromosomeFactory<int, FITNESS_TYPE>>(*this->_chromosomeFactory);

        if (!mvcChromosomeFactory)
        {
            throw new std::runtime_error("Incompatible Chromosome Factory and Default Crossover Strategy");
        }

        std::shared_ptr<mutation::BaseMutationOperation<FITNESS_TYPE>> mutationOperation(
                new mutation::UniformMutationOperation<int, FITNESS_TYPE>(mutationSettings, mutationChoosingOperation, mvcChromosomeFactory));

        return mutationOperation;
    }

    virtual std::unique_ptr<BaseGeneticAlgorithm<FITNESS_TYPE>> build() override
    {
        if (!this->_stoppingCriterion)
        {
            this->_stoppingCriterion = getDefaultStoppingCriterion();
        }

        if (!this->_selectionOperation)
        {
            this->_selectionOperation = getDefaultSelectionOperation();
        }

        if (!this->_couplingOperation)
        {
            this->_couplingOperation = getDefaultCouplingOperation();
        }

        if (!this->_crossoverOperation)
        {
            this->_crossoverOperation = getDefaultCrossoverOperation();
        }

        if (!this->_replacementOperation)
        {
            this->_replacementOperation = getDefaultReplacementOperation();
        }

        if (!this->_mutationOperation)
        {
            this->_mutationOperation = getDefaultMutationOperation();
        }

        if (!this->_chromosomeFactory)
        {
            throw new std::runtime_error("One must at least specify a chromosome Factory with a fitness evaluator");
        }

        auto algorithm = std::unique_ptr<SteadyStateAlgorithm<FITNESS_TYPE>>(
             new SteadyStateAlgorithm<FITNESS_TYPE> (*this->_stoppingCriterion   ,
                                                     *this->_selectionOperation  ,
                                                     *this->_couplingOperation   ,
                                                     *this->_crossoverOperation  ,
                                                     *this->_replacementOperation,
                                                     *this->_mutationOperation   ,
                                                     *this->_chromosomeFactory   )
                                          );
        return std::move(algorithm);
    }

};


}
}

#include <geneial/algorithm/SteadyStateAlgorithm.hpp>
