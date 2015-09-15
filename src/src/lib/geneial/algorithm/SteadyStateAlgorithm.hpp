#pragma once

#include <geneial/algorithm/SteadyStateAlgorithm.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
using ::geneial::population::Population;

geneial_export_namespace
{

template<typename FITNESS_TYPE>
void SteadyStateAlgorithm<FITNESS_TYPE>::solve()
{

    using chromosome_container = typename Population<FITNESS_TYPE>::chromosome_container;

    this->_wasStarted = true;

    //Initialize the first population candidate, take whatever has been inserted and fill it up to max size.
    this->_manager->replenishPopulation();

    long long int iterationCounter = 0;

    bool wasReached = this->wasCriteriaReached();

    const auto bookkeeper = this->_manager->getBookkeeper();
    while (!wasReached)
    {
        {
            //Notify algorithm observers about new generation change.
            {
                ScopedEvent trace_criteria("TIME_OBSERVERS",*bookkeeper);
                this->notifyObservers(AlgorithmObserver<FITNESS_TYPE>::BEFORE_GENERATION);
            }

            ScopedEvent trace_iteration("TIME_ITERATION",*bookkeeper);

            ++iterationCounter;

            //Let all the chromosomes in the population age...
            {
                ScopedEvent trace_aging("TIME_AGING",*bookkeeper);
                this->_manager->getPopulation().doAge();
            }

            //Select some chromosomes from the population using the specified selection operation
            chromosome_container mating_pool;
            {
                ScopedEvent trace_selection("TIME_SELECTION",*bookkeeper);
                //Perform a selection of mating candidates based on the given strategy.
                mating_pool = this->_selectionOperation->doSelect(this->_manager->getPopulation(), *this->_manager);
            }

            /**
             * Create offspring given the previous selection, this is done by
             * 1.) Coupling i.e., determination which chromosome pairs with one another
             * 2.) Crossover i.e., coupling will call crossover and return children
             *
             * TODO (bewo): Should crossover be decoupled from coupling?
             */
            chromosome_container offspring;
            {
                ScopedEvent trace_coupling("TIME_OFFSPRING",*bookkeeper);
                offspring = this->_couplingOperation->doCopulate(mating_pool, *this->_crossoverOperation, *this->_manager);
            }

            //Mutate some of the offspring chromosomes using the specified mutation operation
            //TODO (bewo): Should mutation operate on the whole population instead?
            {
                ScopedEvent trace_mutation("TIME_MUTATION",*bookkeeper);
                offspring = this->_mutationOperation->doMutate(offspring, *this->_manager);
            }

            //Given the new offspring, replace members of the iteration's starting population with the offspring
            {
                ScopedEvent trace_replacement("TIME_REPLACEMENT",*bookkeeper);
                this->_replacementOperation->doReplace(this->_manager->getPopulation(), mating_pool, offspring, *this->_manager);
            }

            //In the case that the replacement removed too much chromosomes from the population we will replenish the missing chromosomes
            {
                ScopedEvent trace_replenishment("TIME_REPLENISHMENT",*bookkeeper);
                //If we had a deficit, fill up population with fresh chromosomes
                this->_manager->replenishPopulation();
            }

            //Check whether the stopping criteria were met this iteration
            {
                ScopedEvent trace_criteria("TIME_CRITERIA",*bookkeeper);
                wasReached = this->wasCriteriaReached();
            }

            //Notify algorithm observers about new generation change.
            {
                ScopedEvent trace_criteria("TIME_OBSERVERS",*bookkeeper);
                this->notifyObservers(AlgorithmObserver<FITNESS_TYPE>::AFTER_GENERATION);
            }
        }

    }

    this->_wasSolved = true;

}


} /* geneial_export_namespace */
} /* private namespace algorithm */
} /* private namespace geneial */
