#pragma once

#include <geneial/core/operations/selection/UniformRandomSelection.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/utility/Random.h>

#include <map>
#include <cassert>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(selection)
{
using ::geneial::utility::Random;

geneial_export_namespace
{

//TODO (bewo) check whether all this will work with negative fitness values

template<typename FITNESS_TYPE>
typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set UniformRandomSelection<FITNESS_TYPE>::doSelect(
        const Population<FITNESS_TYPE> &population, BaseManager<FITNESS_TYPE> &manager) const
{

    //shorthands for type mess
    typedef typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set result_set;
    typedef typename Population<FITNESS_TYPE>::fitnessmap_const_it const_pop_itr;

    result_set result;

    unsigned int left_select = this->getSettings().getNumberOfParents();

    //TODO (bewo) allow parameter for the best chromosomes to be selected (and skipped here)
    assert(population.getSize() >= left_select);

    while (left_select > 0)
    {
        //TODO (bewo) make this a setting:
        const bool allowDuplicates = false;
        const_pop_itr rnditer;
        do
        {
            rnditer = population.getFitnessMap().begin();

            std::advance(rnditer, Random::generate<int>(0, population.getFitnessMap().size() - 1));

        } while (allowDuplicates || std::find(result.begin(), result.end(), rnditer->second) != result.end());

        left_select--;

        result.emplace_back(rnditer->second);

    }
    return result;
}

} /* geneial_export_namespace */
} /* private namespace selection */
} /* private namespace operation */
} /* private namespace geneial */

