#pragma once

#include <geneial/algorithm/SteadyStateAlgorithm.h>

#include <boost/date_time/posix_time/posix_time.hpp>

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

    long long int iter = 0;

    bool wasReached = this->wasCriteriaReached();

    while (!wasReached)
    {
        boost::posix_time::ptime beforeIteration = boost::posix_time::microsec_clock::local_time();

        ++iter;

        boost::posix_time::ptime beforeAging = boost::posix_time::microsec_clock::local_time();
        this->_manager.getPopulation().doAge();
        boost::posix_time::ptime afterAging = boost::posix_time::microsec_clock::local_time();
        aging += static_cast<double>((afterAging - beforeAging).total_microseconds());

        boost::posix_time::ptime beforeSelection = boost::posix_time::microsec_clock::local_time();
        //Perform a selection of mating candidates based on the given strategy.
        auto mating_pool = this->_selectionOperation.doSelect(this->_manager.getPopulation(), this->_manager);
        boost::posix_time::ptime afterSelection = boost::posix_time::microsec_clock::local_time();
        selection += static_cast<double>((afterSelection - beforeSelection).total_microseconds());

        boost::posix_time::ptime beforeCoupling = boost::posix_time::microsec_clock::local_time();
        auto offspring(this->_couplingOperation.doCopulate(mating_pool, this->_crossoverOperation, this->_manager));
        boost::posix_time::ptime afterCoupling = boost::posix_time::microsec_clock::local_time();
        coupling += static_cast<double>((afterCoupling - beforeCoupling).total_microseconds());

        boost::posix_time::ptime beforeMutation = boost::posix_time::microsec_clock::local_time();
        offspring = this->_mutationOperation.doMutate(offspring, this->_manager);
        boost::posix_time::ptime afterMutation = boost::posix_time::microsec_clock::local_time();
        mutation += static_cast<double>((afterMutation - beforeMutation).total_microseconds());

        //TODO (bewo): Scaling?
        boost::posix_time::ptime beforeReplacement = boost::posix_time::microsec_clock::local_time();
        this->_replacementOperation.doReplace(this->_manager.getPopulation(), mating_pool, offspring, this->_manager);
        boost::posix_time::ptime afterReplacement = boost::posix_time::microsec_clock::local_time();
        replacement += static_cast<double>((afterReplacement - beforeReplacement).total_microseconds());

        boost::posix_time::ptime beforeReplenishment = boost::posix_time::microsec_clock::local_time();
        //If we had a deficit, fill up population with fresh chromosomes
        this->_manager.replenishPopulation();
        boost::posix_time::ptime afterReplenishment = boost::posix_time::microsec_clock::local_time();
        replenishment += static_cast<double>((afterReplenishment - beforeReplenishment).total_microseconds());

        boost::posix_time::ptime beforeCriteria = boost::posix_time::microsec_clock::local_time();
        wasReached = this->wasCriteriaReached();
        boost::posix_time::ptime afterCriteria = boost::posix_time::microsec_clock::local_time();
        criteria += static_cast<double>((afterCriteria - beforeCriteria).total_microseconds());

        boost::posix_time::ptime beforeObservers = boost::posix_time::microsec_clock::local_time();
        this->notifyObservers(AlgorithmObserver<FITNESS_TYPE>::GENERATION_DONE);
        boost::posix_time::ptime afterObservers = boost::posix_time::microsec_clock::local_time();
        observer += static_cast<double>((afterObservers - beforeObservers).total_microseconds());

        boost::posix_time::ptime afterIteration = boost::posix_time::microsec_clock::local_time();
        iteration += static_cast<double>((afterIteration - beforeIteration).total_microseconds());

    }

    this->_wasSolved = true;

    std::cout << "Report: " << std::endl;
    std::cout << "\t avg. Aging:\t\t" << (aging / iter) << "ms" << std::endl;
    std::cout << "\t avg. Selection:\t\t" << (selection / iter) << "ms" << std::endl;
    std::cout << "\t avg. Coupling:\t\t" << (coupling / iter) << "ms" << std::endl;
    std::cout << "\t avg. Mutation:\t\t" << (mutation / iter) << "ms" << std::endl;
    std::cout << "\t avg. Replacement:\t\t" << (replacement / iter) << "ms" << std::endl;
    std::cout << "\t avg. Replenishment:\t\t" << (replenishment / iter) << "ms" << std::endl;
    std::cout << "\t avg. Criteria:\t\t" << (criteria / iter) << "ms" << std::endl;
    std::cout << "\t avg. Observer:\t\t" << (observer / iter) << "ms" << std::endl;
    std::cout << std::endl;
    std::cout << "\t avg. Time Iteration:\t\t" << (iteration / iter) << "ms" << std::endl;

}

} /* namespace algoritm */
} /* namespace geneial */
