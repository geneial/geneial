#pragma once

#include <geneial/namespaces.h>

#include <geneial/algorithm/BaseGeneticAlgorithm.h>
#include <geneial/core/population/management/Bookkeeper.h>

#include <memory>
#include <ostream>
#include <unordered_map>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(algorithm)
    {

        using ::geneial::algorithm::BaseGeneticAlgorithm;
        using ::geneial::population::management::EventData;
        using ::geneial::population::management::EventValueData;
        using ::geneial::population::management::StatisticBookkeeper;

        geneial_export_namespace
        {

            void printTiming(const char *string,
                             const std::unordered_multimap<std::string, std::unique_ptr<EventData>> &events,
                             std::ostream &os);

            template <typename FITNESS_TYPE> class Diagnostics
            {
                const std::shared_ptr<BaseGeneticAlgorithm<FITNESS_TYPE>> _algorithm;
                std::shared_ptr<StatisticBookkeeper> getStatistics() const;
                void analyseEvents(const char *prefix, std::ostream &os);

              protected:
                void instrumentAlgorithm();

              public:
                Diagnostics(const std::shared_ptr<BaseGeneticAlgorithm<FITNESS_TYPE>> &algorithm);

                void analyseAll(std::ostream &os);

                void analyseTimeSpent(std::ostream &os);
                void analyseEventsCoupling(std::ostream &os);
                void analyseEventsCrossover(std::ostream &os);
                void analyseEventsMutation(std::ostream &os);
                void analyseEventsReplacement(std::ostream &os);
                void anaylseEventsSelection(std::ostream &os);
                void anaylseConvergence(std::ostream &os);
            };
        }
    }
}

#include <geneial/algorithm/diagnostics/Diagnostics.hpp>
