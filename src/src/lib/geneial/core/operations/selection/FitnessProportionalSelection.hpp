#pragma once

#include <geneial/core/operations/selection/FitnessProportionalSelection.h>
#include <geneial/utility/Random.h>

#include <cassert>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(selection)
{
using ::geneial::population::Population;
using ::geneial::utility::Random;
using ::geneial::population::management::BaseManager;

geneial_export_namespace
{

template<typename FITNESS_TYPE>
typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set FitnessProportionalSelection<FITNESS_TYPE>::doSelect(
        const Population<FITNESS_TYPE> &population, BaseManager<FITNESS_TYPE> &manager) const
{

    //shorthands for type mess
    typedef std::multimap<FITNESS_TYPE, typename BaseChromosome<FITNESS_TYPE>::ptr> map_type;
    typedef typename BaseChromosome<FITNESS_TYPE>::ptr chrom_ptr_type;
    typedef typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set result_set;

    result_set result;
    result.reserve(this->getSettings().getNumberOfParents()+this->_settings->getNumberSelectBest());

    //Calculate total sum of fitness
    FITNESS_TYPE sum;
		for (const auto& it : population.getFitnessMap())
    {
        sum+= (it.second)->getFitness().get();
    }

		if( sum ==  0)
		{
			std::cout << "NOT FITNESS VALUES !\n\n" << population << std::endl;
			std::cout << *(population.getFitnessMap().cbegin()->second) << std::endl;
		}

    assert(sum > 0);

    //Create a multimap with fitness-proportional probability as key
    map_type sorted_multimap;
    for (const auto& it : population.getFitnessMap())
    {
        const FITNESS_TYPE prop_fitness = (it.second)->getFitness().get() / sum;
        sorted_multimap.insert(std::pair<FITNESS_TYPE, chrom_ptr_type>(prop_fitness, it.second));
    }

    unsigned int left_select = this->getSettings().getNumberOfParents();
    //First, handle elitism, iterate backwards over the sorted multimap and select the best chromosomes.

    //TODO (bewo) OPTIMIZE: using sth like transform(i, j, back_inserter(v), select2nd<MapType::value_type>()); instead ???
    unsigned int elitism_to_select = this->_settings->getNumberSelectBest();

    auto crit = sorted_multimap.rbegin();
    for (; crit != sorted_multimap.rend() && elitism_to_select > 0; ++crit)
    {
        result.emplace_back(crit->second);
        elitism_to_select--;
    }
    //Strip the inserted part from the map
    sorted_multimap.erase(crit.base(), sorted_multimap.end());

    assert(result.size() == this->_settings->getNumberSelectBest());
    left_select -= this->_settings->getNumberSelectBest();
    assert(left_select <= sorted_multimap.size());

    //now select the remainder based on proportional probability.
    while (left_select > 0)
    {
        //wrap around if necessary.
        auto crit = sorted_multimap.rbegin();
        while (crit != sorted_multimap.rend() && left_select > 0)
        {
            chrom_ptr_type chrom = crit->second;
            double prob = (crit->first);
            if (Random::decision(prob))
            {
                //Use it.
                result.emplace_back(chrom);
                left_select--;
                sorted_multimap.erase((++crit).base()); //erase requires normal iterator,
            }
            else
            {
                //Skip this chromosome for now.
                ++crit;
            }
        }
        assert(left_select <= sorted_multimap.size());
    }
    return result;
}

} /* geneial_export_namespace */
} /* private namespace selection */
} /* private namespace operation */
} /* private namespace geneial */
