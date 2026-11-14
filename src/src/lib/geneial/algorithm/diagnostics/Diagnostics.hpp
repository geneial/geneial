#pragma once

#include <geneial/algorithm/diagnostics/Diagnostics.h>

#include <algorithm>
#include <iomanip>
#include <map>
#include <memory>
#include <stdexcept>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(algorithm)
    {

        geneial_export_namespace
        {

            template <typename FITNESS_TYPE>
            Diagnostics<FITNESS_TYPE>::Diagnostics(const std::shared_ptr<BaseGeneticAlgorithm<FITNESS_TYPE>> &algorithm)
                : _algorithm(algorithm)
            {
                if (!_algorithm)
                    throw std::invalid_argument("Diagnostics requires an algorithm");
                instrumentAlgorithm();
            }

            template <typename FITNESS_TYPE>
            std::shared_ptr<StatisticBookkeeper> Diagnostics<FITNESS_TYPE>::getStatistics() const
            {
                auto statistics = std::dynamic_pointer_cast<StatisticBookkeeper>(_algorithm->getBookkeeper());
                if (!statistics)
                    throw std::logic_error("Diagnostic bookkeeper has been replaced");
                return statistics;
            }

            template <typename FITNESS_TYPE>
            void Diagnostics<FITNESS_TYPE>::analyseEvents(const char *prefix, std::ostream &os)
            {
                const auto statistics = getStatistics();
                std::map<std::string, std::size_t> counts;
                for (const auto &event : statistics->getEvents())
                    if (event.first.starts_with(prefix))
                        ++counts[event.first];
                os << prefix << " retained event occurrences:\n";
                if (counts.empty())
                    os << "  None retained\n";
                for (const auto &entry : counts)
                    os << "  " << entry.first << " x " << entry.second << '\n';
            }

            template <typename FITNESS_TYPE> void Diagnostics<FITNESS_TYPE>::instrumentAlgorithm()
            {
                if (_algorithm->hasBeenStarted())
                    throw std::logic_error("Attach diagnostics before starting the algorithm");
                if (!std::dynamic_pointer_cast<StatisticBookkeeper>(_algorithm->getBookkeeper()))
                    _algorithm->setBookkeeper(std::make_shared<StatisticBookkeeper>());
            }

            template <typename FITNESS_TYPE> inline void Diagnostics<FITNESS_TYPE>::analyseAll(std::ostream & os)
            {
                analyseTimeSpent(os);
                analyseEventsCoupling(os);
                analyseEventsCrossover(os);
                analyseEventsMutation(os);
                analyseEventsReplacement(os);
                anaylseEventsSelection(os);
                anaylseConvergence(os);
            }

            template <typename FITNESS_TYPE> inline void Diagnostics<FITNESS_TYPE>::analyseTimeSpent(std::ostream & os)
            {
                const auto statkeeper = getStatistics();
                const auto &events = statkeeper->getEvents();

                printTiming("TIME_ITERATION", events, os);
                printTiming("TIME_AGING", events, os);
                printTiming("TIME_SELECTION", events, os);
                printTiming("TIME_OFFSPRING", events, os);
                printTiming("TIME_MUTATION", events, os);
                printTiming("TIME_REPLACEMENT", events, os);
                printTiming("TIME_REPLENISHMENT", events, os);
                printTiming("TIME_CRITERIA", events, os);
                printTiming("TIME_OBSERVERS", events, os);
                os << "Dropped diagnostic events: " << statkeeper->getDroppedEventCount() << '\n';
            }

            template <typename FITNESS_TYPE>
            inline void Diagnostics<FITNESS_TYPE>::analyseEventsCoupling(std::ostream & os)
            {
                analyseEvents("COUPLING_", os);
            }

            template <typename FITNESS_TYPE>
            inline void Diagnostics<FITNESS_TYPE>::analyseEventsCrossover(std::ostream & os)
            {
                analyseEvents("CROSSOVER_", os);
            }

            template <typename FITNESS_TYPE>
            inline void Diagnostics<FITNESS_TYPE>::analyseEventsMutation(std::ostream & os)
            {
                analyseEvents("MUTATION_", os);
            }

            template <typename FITNESS_TYPE>
            inline void Diagnostics<FITNESS_TYPE>::analyseEventsReplacement(std::ostream & os)
            {
                analyseEvents("REPLACEMENT_", os);
            }

            template <typename FITNESS_TYPE>
            inline void Diagnostics<FITNESS_TYPE>::anaylseEventsSelection(std::ostream & os)
            {
                analyseEvents("SELECTION_", os);
            }

            template <typename FITNESS_TYPE>
            inline void Diagnostics<FITNESS_TYPE>::anaylseConvergence(std::ostream & os)
            {
                const auto &population = _algorithm->getPopulation();
                const auto &scores = population.getFitnessMap();
                os << "Current population snapshot (not a convergence history): generation " << population.getAge()
                   << ", size " << scores.size();
                if (!scores.empty())
                    os << ", worst " << scores.begin()->first << ", best " << scores.rbegin()->first;
                os << '\n';
            }
        }
    }
}
