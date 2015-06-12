#pragma once

#include <geneial/algorithm/diagnostics/Diagnostics.h>

#include <memory>
#include <algorithm>
#include <iomanip>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{

geneial_export_namespace
{

template<typename FITNESS_TYPE>
Diagnostics<FITNESS_TYPE>::Diagnostics(const std::shared_ptr<BaseGeneticAlgorithm<FITNESS_TYPE>> &algorithm)
:_algorithm(algorithm)
{
    instrumentAlgorithm();
}


template<typename FITNESS_TYPE>
void Diagnostics<FITNESS_TYPE>::instrumentAlgorithm()
{
    //Have we already done something?
    if (!_algorithm->hasBeenStarted())
    {
        //Inject Statistical Bookkeeper
        //Prepare Algorithm for Diagnosis
        if (!std::dynamic_pointer_cast<StatisticBookkeeper>(_algorithm->getBookkeeper()))
        {
            _algorithm->setBookkeeper(std::shared_ptr<StatisticBookkeeper>(new StatisticBookkeeper));
        }
    }
    else
    {
        std::runtime_error("Algorithm has already started! Unable to inject diagnosis instrumentation!");
    }
}





template<typename FITNESS_TYPE>
inline void Diagnostics<FITNESS_TYPE>::analyseAll(std::ostream& os)
{
    analyseTimeSpent(os);
}

template<typename FITNESS_TYPE>
inline void Diagnostics<FITNESS_TYPE>::analyseTimeSpent(std::ostream& os)
{
    auto statkeeper = std::dynamic_pointer_cast<StatisticBookkeeper>(_algorithm->getBookkeeper());
    auto& events = statkeeper->getEvents();

//    std::sort (events.begin(), events.end());

    printTiming("TIME_ITERATION",events,os);
    printTiming("TIME_AGING",events,os);
    printTiming("TIME_SELECTION",events,os);
    printTiming("TIME_OFFSPRING",events,os);
    printTiming("TIME_MUTATION",events,os);
    printTiming("TIME_REPLACEMENT",events,os);
    printTiming("TIME_REPLENISHMENT",events,os);
    printTiming("TIME_CRITERIA",events,os);
    printTiming("TIME_OBSERVERS",events,os);
}

template<typename FITNESS_TYPE>
inline void Diagnostics<FITNESS_TYPE>::analyseEventsCoupling(std::ostream& os)
{
}

template<typename FITNESS_TYPE>
inline void Diagnostics<FITNESS_TYPE>::analyseEventsCrossover(std::ostream& os)
{
}

template<typename FITNESS_TYPE>
inline void Diagnostics<FITNESS_TYPE>::analyseEventsMutation(std::ostream& os)
{
}

template<typename FITNESS_TYPE>
inline void Diagnostics<FITNESS_TYPE>::analyseEventsReplacement(std::ostream& os)
{
}

template<typename FITNESS_TYPE>
inline void Diagnostics<FITNESS_TYPE>::anaylseEventsSelection(std::ostream& os)
{
}

template<typename FITNESS_TYPE>
inline void Diagnostics<FITNESS_TYPE>::anaylseConvergence(std::ostream& os)
{
}

}
}
}
