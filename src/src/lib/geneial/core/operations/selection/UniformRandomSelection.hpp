#pragma once
#include <geneial/core/operations/selection/UniformRandomSelection.h>
#include <geneial/utility/Random.h>
#include <stdexcept>

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
                UniformRandomSelection<FITNESS_TYPE>::doSelect(const Population<FITNESS_TYPE> &population,
                                                               BaseManager<FITNESS_TYPE> &manager) const
                {
                    const auto count = this->getSettings().getNumberOfParents();
                    if (count > population.getSize())
                        throw std::invalid_argument("Parent count exceeds population size");
                    typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set result;
                    for (const auto &entry : population.getFitnessMap())
                        result.push_back(entry.second);
                    geneial::utility::Random::shuffle(result.begin(), result.end());
                    result.resize(count);
                    return result;
                }
            }
        }
    }
}
