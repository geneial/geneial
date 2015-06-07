#include <geneial/algorithm/diagnostics/Diagnostics.h>

#include <memory>
#include <algorithm>
#include <iomanip>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
    using ::geneial::population::management::EventData;
    using ::geneial::population::management::EventValueData;

using ::geneial::algorithm::BaseGeneticAlgorithm;
using ::geneial::population::management::StatisticBookkeeper;


void printTiming(const char* string, std::unordered_multimap<std::string, std::unique_ptr<EventData> > &events, std::ostream& os)
{
    auto range = events.equal_range(string);
    //Calculate cumulative moving average:
       double cma = 0;
       int n = 0;
       double min = -1;
       double max = 0;
       std::for_each (
           range.first, //first
           range.second, //last
           [&os,&cma,&n,&min,&max](
                   std::pair<const std::string, std::unique_ptr<EventData>> &event)
               {
                   //static int n = 0;
                   EventValueData<double> *derivedPointer = dynamic_cast<EventValueData<double>*>(event.second.get());
                   const double time = derivedPointer->getValue();
                   max = std::max(max,time);
                   if(min == -1)
                   {
                       min = derivedPointer->getValue();
                   }
                   else
                   {
                       min = std::min(min,time);
                   }
                   cma = (n * cma + derivedPointer->getValue()) / (n + 1);
                   //cma += derivedPointer->getValue();
                   n++;
               }
         );
       //cma = (cma ) / n;
       os << " ("<< std::setw(20) <<string<<") x "<<std::setw(12)<<n
               << ",\t Average  "
               << std::setprecision(6) << std::fixed
               << std::setw(12) << cma << " [ms],\t Min " << std::setprecision(6) << std::fixed
               << std::setw(12) << min << " [ms],\t Max " << std::setprecision(6) << std::fixed
               << std::setw(12) << max << " [ms]" << std::endl;
}


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

}
}
}
