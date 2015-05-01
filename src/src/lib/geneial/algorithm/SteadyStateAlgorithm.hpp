#pragma once

#include <geneial/algorithm/SteadyStateAlgorithm.h>

namespace geneial
{
namespace algorithm
{


template<typename FITNESS_TYPE>
void SteadyStateAlgorithm<FITNESS_TYPE>::solve()
{

    this->_wasStarted = true;

    //Initialize the first population candidate, take whatever has been inserted and fill it up to max size.
    this->_manager.replenishPopulation();

    //std::cout << _manager.getPopulation() << std::endl;

    while (!this->wasCriteriaReached())
    {
        this->_manager.getPopulation().doAge();

        //Perform a selection of mating candidates based on the given strategy.
        auto matingPool(this->_selectionOperation.doSelect(this->_manager.getPopulation(), this->_manager));

        auto offspring(this->_couplingOperation.doCopulate(matingPool, this->_crossoverOperation, this->_manager));

        //TODO (bewo): Think this through and check perfect forwarding
        //for container input and move return semantics of mutation return container.
        auto mutatedOffspring(this->_mutationOperation.doMutate(offspring, this->_manager));

        //TODO (bewo): Scaling hook?

        this->_replacementOperation.doReplace(this->_manager.getPopulation(), matingPool, mutatedOffspring, this->_manager);

        //In we had a deficit, fill up population with fresh chromosomes
        this->_manager.replenishPopulation();

        this->notifyObservers(AlgorithmObserver<FITNESS_TYPE>::GENERATION_DONE);
    }

    this->_wasSolved = true;

}


} /* namespace algoritm */
} /* namespace geneial */
