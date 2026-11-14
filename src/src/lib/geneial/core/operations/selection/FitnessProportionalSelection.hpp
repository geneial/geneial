#pragma once

#include <geneial/core/operations/selection/FitnessProportionalSelection.h>
#include <geneial/core/operations/selection/RouletteWheelSelection.h>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(operation)
    {
        geneial_private_namespace(selection)
        {
            geneial_export_namespace
            {

                template <typename FITNESS_TYPE>
                typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set
                FitnessProportionalSelection<FITNESS_TYPE>::doSelect(const Population<FITNESS_TYPE> &population,
                                                                     BaseManager<FITNESS_TYPE> &manager) const
                {
                    const auto count = this->getSettings().getNumberOfParents();
                    const auto eliteCount = this->getSettings().getNumberSelectBest();
                    if (eliteCount > count || count > population.getSize())
                    {
                        throw std::invalid_argument("Invalid parent or elite count");
                    }
                    typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set result;
                    if (count == 0)
                    {
                        return result;
                    }
                    RouletteWheel<FITNESS_TYPE> wheel(population);
                    result.reserve(count);
                    auto elite = population.getFitnessMap().rbegin();
                    for (unsigned int index = 0; index < eliteCount; ++index, ++elite)
                    {
                        result.emplace_back(elite->second);
                        wheel.remove(elite->second);
                    }
                    while (result.size() < count)
                    {
                        auto chromosome = wheel.spin(Random::generate<double>());
                        result.emplace_back(chromosome);
                        wheel.remove(chromosome);
                    }
                    return result;
                }
            }
        }
    }
}
