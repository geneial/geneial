#pragma once

#include <geneial/algorithm/SteadyStateAlgorithm.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
using ::geneial::algorithm::stopping_criteria::BaseStoppingCriterion;
using ::geneial::algorithm::stopping_criteria::MaxGenerationCriterion;

using ::geneial::operation::selection::BaseSelectionOperation;
using ::geneial::operation::selection::RouletteWheelSelection;
using ::geneial::operation::selection::SelectionSettings;

using ::geneial::operation::coupling::BaseCouplingOperation;
using ::geneial::operation::coupling::CouplingSettings;
using ::geneial::operation::coupling::RandomCouplingOperation;

using ::geneial::operation::crossover::BaseCrossoverOperation;
using ::geneial::operation::crossover::MultiValueChromosomeNPointCrossover;
using ::geneial::operation::crossover::MultiValueChromosomeNPointCrossoverSettings;

using ::geneial::operation::replacement::BaseReplacementOperation;
using ::geneial::operation::replacement::BaseReplacementSettings;
using ::geneial::operation::replacement::ReplaceWorstOperation;

using ::geneial::operation::mutation::BaseMutationOperation;
using ::geneial::operation::mutation::MutationSettings;
using ::geneial::operation::choosing::ChooseRandom;
using ::geneial::operation::mutation::UniformMutationOperation;

using ::geneial::population::chromosome::BaseChromosomeFactory;
using ::geneial::population::chromosome::MultiValueChromosomeFactory;

geneial_export_namespace
{

template<typename FITNESS_TYPE>
std::shared_ptr<BaseStoppingCriterion<FITNESS_TYPE>> SteadyStateAlgorithm<FITNESS_TYPE>::Builder::getDefaultStoppingCriterion() const
{
    std::shared_ptr<BaseStoppingCriterion<FITNESS_TYPE>> stoppingCriterion(
            new MaxGenerationCriterion<FITNESS_TYPE>(100000));
    return stoppingCriterion;
}


template<typename FITNESS_TYPE>
std::shared_ptr<BaseSelectionOperation<FITNESS_TYPE>> SteadyStateAlgorithm<FITNESS_TYPE>::Builder::getDefaultSelectionOperation() const
{
        std::shared_ptr<BaseSelectionOperation<FITNESS_TYPE>> selectionOperation(
                new RouletteWheelSelection<FITNESS_TYPE>(
                        std::shared_ptr < SelectionSettings > (new SelectionSettings(5))));
        return selectionOperation;
}


template<typename FITNESS_TYPE>
std::shared_ptr<BaseCouplingOperation<FITNESS_TYPE>> SteadyStateAlgorithm<FITNESS_TYPE>::Builder::getDefaultCouplingOperation() const
{

    std::shared_ptr<CouplingSettings> couplingSettings(new CouplingSettings(20));

    std::shared_ptr<BaseCouplingOperation<FITNESS_TYPE>> couplingOperation(
            new RandomCouplingOperation<FITNESS_TYPE>(couplingSettings));

    return couplingOperation;
}


template<typename FITNESS_TYPE>
std::shared_ptr<BaseCrossoverOperation<FITNESS_TYPE>> SteadyStateAlgorithm<FITNESS_TYPE>::Builder::getDefaultCrossoverOperation() const
{

    //In this case we need to check whether the chosen chromomsome builder is compatible with our default operation:
    auto mvcChromosomeFactory = std::dynamic_pointer_cast<MultiValueChromosomeFactory<int, FITNESS_TYPE>>(
            *this->_chromosomeFactory);

    if (!mvcChromosomeFactory)
    {
        throw new std::runtime_error("Incompatible Chromosome Factory and default crossover strategy");
    }

    auto crossoverSettings = std::shared_ptr < MultiValueChromosomeNPointCrossoverSettings
            > (new MultiValueChromosomeNPointCrossoverSettings(1,
                    MultiValueChromosomeNPointCrossoverSettings::RANDOM_WIDTH, 1));

    std::shared_ptr<BaseCrossoverOperation<FITNESS_TYPE>> crossoverOperation(
            new MultiValueChromosomeNPointCrossover<int, FITNESS_TYPE>(crossoverSettings,
                    mvcChromosomeFactory));

    return crossoverOperation;
}


template<typename FITNESS_TYPE>
std::shared_ptr<BaseReplacementOperation<FITNESS_TYPE>> SteadyStateAlgorithm<FITNESS_TYPE>::Builder::getDefaultReplacementOperation() const
{
    //TODO (bewo) derive to replace from Population Settings?!

    std::shared_ptr<BaseReplacementSettings> replacementSettings(
            new BaseReplacementSettings(BaseReplacementSettings::REPLACE_ALL_OFFSPRING, 20,
                    0));

    std::shared_ptr<ReplaceWorstOperation<FITNESS_TYPE>> replacementOperation(
            new ReplaceWorstOperation<FITNESS_TYPE>(replacementSettings));

    return replacementOperation;
}

template<typename FITNESS_TYPE>
std::shared_ptr<BaseMutationOperation<FITNESS_TYPE>> SteadyStateAlgorithm<FITNESS_TYPE>::Builder::getDefaultMutationOperation() const
{

    std::shared_ptr<MutationSettings> mutationSettings(new MutationSettings(0.1, 0.1, 5));

    std::shared_ptr<ChooseRandom<int, FITNESS_TYPE>> mutationChoosingOperation( new ChooseRandom<int, FITNESS_TYPE>(mutationSettings));

    //In this case we need to check whether the chosen chromomsome builder is compatible with our default operation:
    auto mvcChromosomeFactory = std::dynamic_pointer_cast<MultiValueChromosomeFactory<int, FITNESS_TYPE>>(*this->_chromosomeFactory);

    if (!mvcChromosomeFactory)
    {
        throw new std::runtime_error("Incompatible Chromosome Factory and Default Crossover Strategy");
    }

    std::shared_ptr<BaseMutationOperation<FITNESS_TYPE>> mutationOperation(
            new UniformMutationOperation<int, FITNESS_TYPE>(mutationSettings, mutationChoosingOperation, mvcChromosomeFactory));

    return mutationOperation;
}


template<typename FITNESS_TYPE>
std::unique_ptr<BaseGeneticAlgorithm<FITNESS_TYPE>> SteadyStateAlgorithm<FITNESS_TYPE>::Builder::build()
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

    auto algorithm = std::unique_ptr < SteadyStateAlgorithm
            < FITNESS_TYPE
                    >> (new SteadyStateAlgorithm<FITNESS_TYPE>(*this->_stoppingCriterion, *this->_selectionOperation,
                            *this->_couplingOperation, *this->_crossoverOperation, *this->_replacementOperation,
                            *this->_mutationOperation, *this->_chromosomeFactory));
    return std::move(algorithm);
}


} /* geneial_export_namespace */
} /* private namespace algorithm */
} /* private namespace geneial */
