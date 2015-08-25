#pragma once

#include <geneial/namespaces.h>
#include <geneial/algorithm/BaseGeneticAlgorithm.h>

#include <geneial/core/operations/replacement/ReplaceWorstOperation.h>
#include <geneial/core/operations/mutation/UniformMutationOperation.h>
#include <geneial/core/operations/choosing/ChooseRandom.h>
#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>
#include <geneial/core/operations/selection/RouletteWheelSelection.h>
#include <geneial/core/operations/coupling/RandomCouplingOperation.h>

#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>

#include <geneial/utility/mixins/EnableMakeShared.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
using ::geneial::operation::selection::BaseSelectionOperation;
using ::geneial::operation::coupling::BaseCouplingOperation;
using ::geneial::operation::crossover::BaseCrossoverOperation;
using ::geneial::operation::replacement::BaseReplacementOperation;
using ::geneial::operation::mutation::BaseMutationOperation;
using ::geneial::population::chromosome::BaseChromosomeFactory;
using ::geneial::algorithm::stopping_criteria::BaseStoppingCriterion;

using ::geneial::population::management::ScopedEvent;
using ::geneial::utility::EnableMakeShared;


geneial_export_namespace
{

template<typename FITNESS_TYPE>
class SteadyStateAlgorithm: public BaseGeneticAlgorithm<FITNESS_TYPE>,
                            public virtual EnableMakeShared<SteadyStateAlgorithm<FITNESS_TYPE>>
{
protected:
    SteadyStateAlgorithm(
                std::shared_ptr<BaseStoppingCriterion<FITNESS_TYPE>> stoppingCriterion,
                std::shared_ptr<BaseSelectionOperation<FITNESS_TYPE>> selectionOperation,
                std::shared_ptr<BaseCouplingOperation<FITNESS_TYPE>> couplingOperation,
                std::shared_ptr<BaseCrossoverOperation<FITNESS_TYPE>> crossoverOperation,
                std::shared_ptr<BaseReplacementOperation<FITNESS_TYPE>> replacementOperation,
                std::shared_ptr<BaseMutationOperation<FITNESS_TYPE>> mutationOperation,
                std::shared_ptr<BaseChromosomeFactory<FITNESS_TYPE>> chromosomeFactory) :
                BaseGeneticAlgorithm<FITNESS_TYPE>(stoppingCriterion,
                                                   selectionOperation,
                                                   couplingOperation,
                                                   crossoverOperation,
                                                   replacementOperation,
                                                   mutationOperation,
                                                   chromosomeFactory)
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
    virtual std::shared_ptr<BaseStoppingCriterion<FITNESS_TYPE>> getDefaultStoppingCriterion() const;

    virtual std::shared_ptr<BaseSelectionOperation<FITNESS_TYPE>> getDefaultSelectionOperation() const;

    virtual std::shared_ptr<BaseCouplingOperation<FITNESS_TYPE>> getDefaultCouplingOperation() const;

    virtual std::shared_ptr<BaseCrossoverOperation<FITNESS_TYPE>> getDefaultCrossoverOperation() const;

    virtual std::shared_ptr<BaseReplacementOperation<FITNESS_TYPE>> getDefaultReplacementOperation() const;

    virtual std::shared_ptr<BaseMutationOperation<FITNESS_TYPE>> getDefaultMutationOperation() const;

    virtual std::shared_ptr<BaseGeneticAlgorithm<FITNESS_TYPE>> create() override;

};



} /* geneial_export_namespace */
} /* private namespace algorithm */
} /* private namespace geneial */


#include <geneial/algorithm/SteadyStateAlgorithm.hpp>
#include <geneial/algorithm/SteadyStateAlgorithmBuilder.hpp>
