#pragma once

#include <geneial/core/operations/replacement/ReplaceWorstOperation.h>
#include <geneial/core/population/management/Bookkeeper.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <tuple>
#include <utility>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(operation)
    {
        geneial_private_namespace(replacement)
        {
            using ::geneial::operation::coupling::BaseCouplingOperation;
            using ::geneial::operation::selection::BaseSelectionOperation;
            using ::geneial::population::Population;
            using ::geneial::population::management::EventValueData;

            geneial_export_namespace
            {

                template <typename FITNESS_TYPE>
                unsigned int ReplaceWorstOperation<FITNESS_TYPE>::getAmountToReplace(
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
                void ReplaceWorstOperation<FITNESS_TYPE>::doReplace(
                    Population<FITNESS_TYPE> & population,
                    const typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &parents,
                    typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring,
                    BaseManager<FITNESS_TYPE> &manager) const
                {
                    unsigned int numberToReplace =
                        getAmountToReplace(population, offspring); // this also takes care of elitism!

                    // We'd rather insert the chromsomes all at one, so we can use multithreading for determining the
                    // fitness. However, first we need to check whehter we will have duplicates

                    typename Population<FITNESS_TYPE>::chromosome_container newChildren = offspring;
                    const auto duplicates = population.removeDuplicates(newChildren);
                    newChildren.resize(std::min<std::size_t>(newChildren.size(), numberToReplace));
                    const auto childrenCandidates = static_cast<unsigned int>(newChildren.size());

                    const unsigned int failed_inserts =
                        duplicates + childrenCandidates - population.insertChromosomeContainer(newChildren);

                    EventValueData<unsigned int>::create(*manager.getBookkeeper(), "REPLACEMENT_FAILED_INSERTS",
                                                         failed_inserts);

                    // we might have a deficit at this point if offsprings were already contained.

                    // remove the worst chromosomes to replace (assuming worst is at the very beginning)
                    typename Population<FITNESS_TYPE>::fitnessmap_const_it advanced =
                        population.getFitnessMap().cbegin();

                    const unsigned int maximum = manager.getPopulationSettings().getMaxChromosomes();
                    const unsigned int worstChromosomesToRemove =
                        population.getSize() > maximum ? population.getSize() - maximum : 0;
                    if (worstChromosomesToRemove != 0)
                    {
                        std::advance(advanced, worstChromosomesToRemove);

                        typename Population<FITNESS_TYPE>::chromosome_container toRemove;
                        toRemove.reserve(numberToReplace);

                        // insert the second component of the fitness map (the chromosome ptr) into toRemove
                        std::transform(population.getFitnessMap().cbegin(), advanced, std::back_inserter(toRemove),
                                       [](decltype(*advanced) &p) { return p.second; });
                        population.removeChromosomeContainer(toRemove);
                    }
                    else
                    {
                        manager.getBookkeeper()->traceEvent("REPLACEMENT_NO_WORST_DELETED");
                    }
                }

            } /* geneial_export_namespace */
        } /* private namespace replacement */
    } /* private namespace operation */
} /* private namespace geneial */
