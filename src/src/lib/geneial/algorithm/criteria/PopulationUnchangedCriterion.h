#pragma once

#include <algorithm>
#include <geneial/algorithm/criteria/StatefulStoppingCriterion.h>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(algorithm)
    {
        geneial_private_namespace(stopping_criteria)
        {
            using ::geneial::population::management::BaseManager;
            geneial_export_namespace
            {
                /** Compares genome membership, ignoring age/fitness. Keep observed genomes immutable until reset. */
                template <typename FITNESS_TYPE>
                class PopulationUnchangedCriterion : public StatefulStoppingCriterion<FITNESS_TYPE>
                {
                  public:
                    void reset() override
                    {
                        StatefulStoppingCriterion<FITNESS_TYPE>::reset();
                        _observed = false;
                        _previous.clear();
                    }

                    bool wasStatefullyReached(BaseManager<FITNESS_TYPE> &manager) override
                    {
                        const auto &population = manager.getPopulation();
                        const bool unchanged = _observed && _previous.size() == population.getSize() &&
                                               std::all_of(_previous.begin(), _previous.end(),
                                                           [&population](const auto &entry) {
                                                               return entry.first == entry.second->getHash() &&
                                                                      population.containsChromosome(entry.second);
                                                           });
                        _previous = population.getHashMap();
                        _observed = true;
                        return unchanged;
                    }

                    void print(std::ostream &stream) const override
                    {
                        stream << "PopulationUnchanged";
                    }

                  private:
                    bool _observed = false;
                    typename geneial::population::Population<FITNESS_TYPE>::hash_map _previous;
                };
            }
        }
    }
}
