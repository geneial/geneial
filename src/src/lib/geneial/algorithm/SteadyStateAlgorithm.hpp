#pragma once

#include <geneial/algorithm/SteadyStateAlgorithm.h>

#include <boost/date_time/posix_time/posix_time.hpp>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
geneial_export_namespace
{

template<typename FITNESS_TYPE>
void SteadyStateAlgorithm<FITNESS_TYPE>::solve()
{

    using boost::posix_time::ptime;
    using boost::posix_time::microsec_clock;

    this->_wasStarted = true;

    //Initialize the first population candidate, take whatever has been inserted and fill it up to max size.
    this->_manager->replenishPopulation();

    //  std::cout << _manager.getPopulation() << std::endl;
    long double aging = 0;

    long double selection = 0;

    long double coupling = 0;

    long double mutation = 0;

    long double replacement = 0;

    long double replenishment = 0;

    long double criteria = 0;

    long double observer = 0;

    long double iteration = 0;

    long long int iterationCounter = 0;

    bool wasReached = this->wasCriteriaReached();

    while (!wasReached)
    {

ptime beforeIteration = microsec_clock::local_time();

        ++iterationCounter;

ptime beforeAging = microsec_clock::local_time();

        this->_manager->getPopulation().doAge();

ptime afterAging = microsec_clock::local_time();
aging += static_cast<double>((afterAging - beforeAging).total_microseconds());



ptime beforeSelection = microsec_clock::local_time();

        //Perform a selection of mating candidates based on the given strategy.
        auto mating_pool = this->_selectionOperation->doSelect(this->_manager->getPopulation(), *this->_manager);

ptime afterSelection = microsec_clock::local_time();
selection += static_cast<double>((afterSelection - beforeSelection).total_microseconds());



ptime beforeCoupling = microsec_clock::local_time();

        auto offspring(this->_couplingOperation->doCopulate(mating_pool, *this->_crossoverOperation, *this->_manager));

ptime afterCoupling = microsec_clock::local_time();
coupling += static_cast<double>((afterCoupling - beforeCoupling).total_microseconds());



ptime beforeMutation = microsec_clock::local_time();

        offspring = this->_mutationOperation->doMutate(offspring, *this->_manager);

ptime afterMutation = microsec_clock::local_time();
mutation += static_cast<double>((afterMutation - beforeMutation).total_microseconds());



        //TODO (bewo): Scaling?
ptime beforeReplacement = microsec_clock::local_time();

        this->_replacementOperation->doReplace(this->_manager->getPopulation(), mating_pool, offspring, *this->_manager);

ptime afterReplacement = microsec_clock::local_time();
replacement += static_cast<double>((afterReplacement - beforeReplacement).total_microseconds());



ptime beforeReplenishment = microsec_clock::local_time();

        //If we had a deficit, fill up population with fresh chromosomes
        this->_manager->replenishPopulation();

ptime afterReplenishment = microsec_clock::local_time();
replenishment += static_cast<double>((afterReplenishment - beforeReplenishment).total_microseconds());



ptime beforeCriteria = microsec_clock::local_time();

        wasReached = this->wasCriteriaReached();

ptime afterCriteria = microsec_clock::local_time();
criteria += static_cast<double>((afterCriteria - beforeCriteria).total_microseconds());



ptime beforeObservers = microsec_clock::local_time();

        this->notifyObservers(AlgorithmObserver<FITNESS_TYPE>::GENERATION_DONE);

ptime afterObservers = microsec_clock::local_time();
observer += static_cast<double>((afterObservers - beforeObservers).total_microseconds());




ptime afterIteration = microsec_clock::local_time();
iteration += static_cast<double>((afterIteration - beforeIteration).total_microseconds());


    }

    this->_wasSolved = true;

    std::cout << "Report: " << std::endl;
    std::cout << "\t avg. Aging:\t\t" << (aging / iterationCounter) << "ms" << std::endl;
    std::cout << "\t avg. Selection:\t\t" << (selection / iterationCounter) << "ms" << std::endl;
    std::cout << "\t avg. Coupling:\t\t" << (coupling / iterationCounter) << "ms" << std::endl;
    std::cout << "\t avg. Mutation:\t\t" << (mutation / iterationCounter) << "ms" << std::endl;
    std::cout << "\t avg. Replacement:\t\t" << (replacement / iterationCounter) << "ms" << std::endl;
    std::cout << "\t avg. Replenishment:\t\t" << (replenishment / iterationCounter) << "ms" << std::endl;
    std::cout << "\t avg. Criteria:\t\t" << (criteria / iterationCounter) << "ms" << std::endl;
    std::cout << "\t avg. Observer:\t\t" << (observer / iterationCounter) << "ms" << std::endl;
    std::cout << std::endl;
    std::cout << "\t avg. Time Iteration:\t\t" << (iteration / iterationCounter) << "ms" << std::endl;

}


} /* geneial_export_namespace */
} /* private namespace algorithm */
} /* private namespace geneial */
