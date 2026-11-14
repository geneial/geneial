#pragma once

#include <geneial/core/operations/replacement/ReplaceRandomOperation.h>
#include <geneial/utility/Random.h>

#include <algorithm>
#include <iterator>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(operation)
    {
        geneial_private_namespace(replacement)
        {
            using ::geneial::operation::coupling::BaseCouplingOperation;
            using ::geneial::operation::selection::BaseSelectionOperation;
            using ::geneial::population::Population;
            using ::geneial::utility::Random;

            geneial_export_namespace
            {
                template <typename FITNESS_TYPE>
                unsigned int ReplaceRandomOperation<FITNESS_TYPE>::getAmountToReplace(
                    const Population<FITNESS_TYPE> &population,
                    const typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring) const
                {
                    const std::size_t size = population.getSize();
                    const std::size_t elites = this->getSettings().getAmountElitism();
                    const std::size_t available = size > elites ? size - elites : 0;
                    const std::size_t requested =
                        this->getSettings().getMode() == BaseReplacementSettings::REPLACE_ALL_OFFSPRING
                            ? offspring.size()
                            : this->getSettings().getAmountToReplace();
                    return static_cast<unsigned int>(std::min({available, requested, offspring.size()}));
                }

                template <typename FITNESS_TYPE>
                void ReplaceRandomOperation<FITNESS_TYPE>::doReplace(
                    Population<FITNESS_TYPE> & population,
                    const typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &parents,
                    typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring,
                    BaseManager<FITNESS_TYPE> &manager) const
                {
                    const auto budget = getAmountToReplace(population, offspring);
                    if (budget == 0)
                        return;
                    auto children = offspring;
                    population.removeDuplicates(children);
                    children.resize(std::min<std::size_t>(children.size(), budget));
                    if (children.empty())
                        return;

                    typename Population<FITNESS_TYPE>::chromosome_container candidates;
                    const auto available = population.getSize() - this->getSettings().getAmountElitism();
                    auto member = population.getFitnessMap().begin();
                    for (std::size_t index = 0; index < available; ++index, ++member)
                        candidates.push_back(member->second);

                    const auto inserted = population.insertChromosomeContainer(children);
                    Random::shuffle(candidates.begin(), candidates.end());
                    candidates.resize(inserted);
                    population.removeChromosomeContainer(candidates);
                }
            }
        }
    }
}
