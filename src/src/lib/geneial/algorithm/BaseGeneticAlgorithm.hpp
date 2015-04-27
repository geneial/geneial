#pragma once

#include <geneial/algorithm/BaseGeneticAlgorithm.h>

#include <iterator>
#include <set>

namespace geneial
{
namespace algorithm
{

template<typename FITNESS_TYPE>
void BaseGeneticAlgorithm<FITNESS_TYPE>::solve()
{

    _wasStarted = true;

    //Initialize the first population candidate, take whatever has been inserted and fill it up to max size.
    _manager.replenishPopulation();

    //	std::cout << _manager.getPopulation() << std::endl;

    //Initial check
    while (!wasCriteriaReached())
    {
        _manager.getPopulation().doAge();

        //std::cout << _manager.getPopulation() << std::endl;

        //Perform a selection of mating candidates based on the given strategy.
        typename selection::BaseSelectionOperation<FITNESS_TYPE>::selection_result_set mating_pool; //<-- TODO (bewo) (use shared_ptr here?)
        mating_pool = _selectionOperation->doSelect(_manager.getPopulation(), _manager);

        typename coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set offspring;
        offspring = _couplingOperation->doCopulate(mating_pool, _crossoverOperation, _manager);
        offspring = _mutationOperation->doMutate(offspring, _manager);

        //TODO (bewo): Scaling?

        _replacementOperation->doReplace(_manager.getPopulation(), mating_pool, offspring, _manager);

        //In we had a deficit, fill up population with fresh chromosomes
        _manager.replenishPopulation();

        notifyObservers(AlgorithmObserver<FITNESS_TYPE>::GENERATION_DONE);
    }

    _wasSolved = true;

}

template<typename FITNESS_TYPE>
inline bool BaseGeneticAlgorithm<FITNESS_TYPE>::wasCriteriaReached() //TODO(bewo) Rethink about constness of this
{
    const bool wasReached = _stoppingCriterion->wasReached(_manager);
    if (wasReached)
    {
        notifyObservers(AlgorithmObserver<FITNESS_TYPE>::CRITERIA_REACHED);
    }
    return wasReached;
}

} /* namespace algorithm */
} /* namespace geneial */

