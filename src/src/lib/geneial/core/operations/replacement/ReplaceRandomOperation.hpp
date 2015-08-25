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
template<typename FITNESS_TYPE>
unsigned int ReplaceRandomOperation<FITNESS_TYPE>::getAmountToReplace(const Population<FITNESS_TYPE> &population,
        const typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring) const
{
    switch (this->getSettings().getMode())
    {

    case BaseReplacementSettings::REPLACE_ALL_OFFSPRING:
    {
        return std::min(population.getFitnessMap().size(), offspring.size());
    }
        break;

    case BaseReplacementSettings::REPLACE_FIXED_NUMBER:
    default:
    {
        return std::min(population.getFitnessMap().size(),
                (typename Population<FITNESS_TYPE>::fitness_map::size_type) this->getSettings().getAmountToReplace());
    }
        break;
    }
}

template<typename FITNESS_TYPE>
void ReplaceRandomOperation<FITNESS_TYPE>::doReplace(Population<FITNESS_TYPE> &population,
        const typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &parents,
        typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring,
        BaseManager<FITNESS_TYPE> &manager) const
{

    unsigned int numberToReplace = getAmountToReplace(population, offspring);

    //Remove random elements from the population w.r.t. elitism
    while (numberToReplace)
    {
        //Ignore the very last chromosomes (elitism):
        const unsigned int max = population.getFitnessMap().size() - this->getSettings().getAmountElitism() - 1;

        //pick a random element to delete:
        const unsigned int rnd_advance = Random::generate<int>(0, max);

        //construct an iterator
        auto advanced = population.getFitnessMap().begin();
        std::advance(advanced, rnd_advance);

        //remove the element
        population.removeChromosome(advanced->second);

        numberToReplace--;
    }

    //Insert all the offspring, parents are ignored here (since they are assumed to be already in the population)
    population.insertChromosomeContainer(offspring);

}

} /* geneial_export_namespace */
} /* private namespace replacement */
} /* private namespace operation */
} /* private namespace geneial */

