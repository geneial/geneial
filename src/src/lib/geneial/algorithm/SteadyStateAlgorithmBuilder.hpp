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
using ::geneial::operation::selection::BaseSelectionSettings;

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
using ::geneial::operation::mutation::MultiValueMutationSettings;
using ::geneial::operation::choosing::ChooseRandom;
using ::geneial::operation::mutation::UniformMutationOperation;

using ::geneial::population::chromosome::BaseChromosomeFactory;
using ::geneial::population::chromosome::MultiValueChromosomeFactory;

geneial_export_namespace
{

template<typename FITNESS_TYPE>
std::shared_ptr<BaseStoppingCriterion<FITNESS_TYPE>>SteadyStateAlgorithm<FITNESS_TYPE>::Builder::getDefaultStoppingCriterion() const
{
    return std::make_shared<MaxGenerationCriterion<FITNESS_TYPE>>(100000);
}


template<typename FITNESS_TYPE>
std::shared_ptr<BaseSelectionOperation<FITNESS_TYPE>> SteadyStateAlgorithm<FITNESS_TYPE>::Builder::getDefaultSelectionOperation() const
{
        return typename RouletteWheelSelection<FITNESS_TYPE>::Builder().create();
}


template<typename FITNESS_TYPE>
std::shared_ptr<BaseCouplingOperation<FITNESS_TYPE>> SteadyStateAlgorithm<FITNESS_TYPE>::Builder::getDefaultCouplingOperation() const
{
    return typename RandomCouplingOperation<FITNESS_TYPE>::Builder().create();
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

    auto builder = typename MultiValueChromosomeNPointCrossover<int,FITNESS_TYPE>::Builder(mvcChromosomeFactory);
    return builder.create();
}


template<typename FITNESS_TYPE>
std::shared_ptr<BaseReplacementOperation<FITNESS_TYPE>> SteadyStateAlgorithm<FITNESS_TYPE>::Builder::getDefaultReplacementOperation() const
{
    return typename ReplaceWorstOperation<FITNESS_TYPE>::Builder().create();
}

template<typename FITNESS_TYPE>
std::shared_ptr<BaseMutationOperation<FITNESS_TYPE>> SteadyStateAlgorithm<FITNESS_TYPE>::Builder::getDefaultMutationOperation() const
{

    //In this case we need to check whether the chosen chromomsome builder is compatible with our default operation:
    auto mvcChromosomeFactory = std::dynamic_pointer_cast<MultiValueChromosomeFactory<int, FITNESS_TYPE>>(*this->_chromosomeFactory);

    if (!mvcChromosomeFactory)
    {
        throw new std::runtime_error("Incompatible Chromosome Factory and Default Crossover Strategy");
    }

    //Build Uniform mutation with all the default values.
    auto mutationBuilder = typename UniformMutationOperation<int,FITNESS_TYPE>::Builder(mvcChromosomeFactory);

    return mutationBuilder.create();
}


template<typename FITNESS_TYPE>
std::shared_ptr<BaseGeneticAlgorithm<FITNESS_TYPE>> SteadyStateAlgorithm<FITNESS_TYPE>::Builder::create()
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

    auto algorithm = SteadyStateAlgorithm<FITNESS_TYPE>::makeShared(*this->_stoppingCriterion,
                                                                    *this->_selectionOperation,
                                                                    *this->_couplingOperation,
                                                                    *this->_crossoverOperation,
                                                                    *this->_replacementOperation,
                                                                    *this->_mutationOperation,
                                                                    *this->_chromosomeFactory);
    return std::move(algorithm);
}


} /* geneial_export_namespace */
} /* private namespace algorithm */
} /* private namespace geneial */
