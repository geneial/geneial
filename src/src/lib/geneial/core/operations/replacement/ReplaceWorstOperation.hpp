#pragma once

#include <geneial/core/operations/replacement/BaseReplacementSettings.h>

#include <algorithm>
#include <iterator>

namespace geneial
{
namespace operation
{
namespace replacement
{

template<typename FITNESS_TYPE>
unsigned int ReplaceWorstOperation<FITNESS_TYPE>::getAmountToReplace(const Population<FITNESS_TYPE> &population,
        const typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring) const
{
    switch (this->getSettings().getMode())
    {

    case BaseReplacementSettings::REPLACE_ALL_OFFSPRING:
    {
        return std::min(population.getFitnessMap().size() - this->getSettings().getAmountElitism() - 1,
                offspring.size());
    }
        break;

    case BaseReplacementSettings::REPLACE_FIXED_NUMBER: /*FALLTHROUGH*/
    default:
    {
        return std::min(population.getFitnessMap().size() - 1,
                (typename Population<FITNESS_TYPE>::fitness_map::size_type) this->getSettings().getAmountToReplace());
    }
        break;
    }
}

template<typename FITNESS_TYPE>
void ReplaceWorstOperation<FITNESS_TYPE>::doReplace(Population<FITNESS_TYPE> &population,
        const typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &parents,
        typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring,
        BaseManager<FITNESS_TYPE> &manager)
{

    unsigned int numberToReplace = getAmountToReplace(population, offspring); //this also takes care of elitism!

    //remove the worst n chromosomes to replace (assuming worst is at the very beginning)
    typename Population<FITNESS_TYPE>::fitnessmap_const_it advanced = population.getFitnessMap().begin();
    std::advance(advanced, numberToReplace);
    typename Population<FITNESS_TYPE>::chromosome_container toRemove;
    //TODO (bewo) use copy?
    for (typename Population<FITNESS_TYPE>::fitnessmap_const_it it = population.getFitnessMap().begin(); it != advanced;
            it++)
    {
        toRemove.push_back(it->second);
    }
    population.removeChromosomeContainer(toRemove);

    //Insert all the offspring, parents are ignored here (since they are assumed to be already in the population)
    population.insertChromosomeContainer(offspring);

}

} /* namespace replacement */
} /* namespace operation */
} /* namespace geneial */

