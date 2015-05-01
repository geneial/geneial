#pragma once

#include <geneial/core/operations/selection/FitnessProportionalSelection.h>
#include <geneial/utility/Random.h>

#include <cassert>

namespace geneial
{
namespace operation
{
namespace selection
{

template<typename FITNESS_TYPE>
typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set FitnessProportionalSelection<FITNESS_TYPE>::doSelect(
        const Population<FITNESS_TYPE> &population, BaseManager<FITNESS_TYPE> &manager)
{

    //shorthands for type mess
    typedef std::multimap<FITNESS_TYPE, typename BaseChromosome<FITNESS_TYPE>::ptr> map_type;
    typedef typename BaseChromosome<FITNESS_TYPE>::ptr chrom_ptr_type;
    typedef typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set result_set;

    result_set result;
    result.reserve(_settings.getNumberOfParents()+_settings.getNumberSelectBest());

    //Calculate total sum of fitness
    FITNESS_TYPE sum = std::accumulate(population.getFitnessMap().cbegin(), population.getFitnessMap().cend(), 0,
            [](FITNESS_TYPE total, const typename Population<FITNESS_TYPE>::fitness_map::value_type& mapdata)
            {
                total += (mapdata.second)->getFitness()->get();
                return total;
            });

    assert(sum > 0);

    //Create a multimap with fitness-proportional probability as key
    map_type sorted_multimap;
    for (const auto& it : population.getFitnessMap())
    {
        FITNESS_TYPE prop_fitness = (it.second)->getFitness()->get() / sum;
        sorted_multimap.insert(std::pair<FITNESS_TYPE, chrom_ptr_type>(prop_fitness, it.second));
    }

    unsigned int left_select = _settings.getNumberOfParents();
    //First, handle elitism, iterate backwards over the sorted multimap and select the best chromosomes.

    //TODO (bewo) OPTIMIZE: using sth like transform(i, j, back_inserter(v), select2nd<MapType::value_type>()); instead ???
    unsigned int elitism_to_select = _settings.getNumberSelectBest();
    typename map_type::reverse_iterator crit = sorted_multimap.rbegin();
    for (; crit != sorted_multimap.rend() && elitism_to_select > 0; ++crit)
    {
        result.push_back(crit->second);
        elitism_to_select--;
    }
    //Strip the inserted part from the map
    sorted_multimap.erase(crit.base(), sorted_multimap.end());

    assert(result.size() == _settings.getNumberSelectBest());
    left_select -= _settings.getNumberSelectBest();
    assert(left_select <= sorted_multimap.size());

    //now select the remainder based on proportional probability.
    while (left_select > 0)
    {
        //wrap around if necessary.
        typename map_type::reverse_iterator crit = sorted_multimap.rbegin();
        while (crit != sorted_multimap.rend() && left_select > 0)
        {
            chrom_ptr_type chrom = crit->second;
            double prob = (crit->first);
            if (Random::decision(prob))
            {
                //Use it.
                result.push_back(chrom);
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

} /* namespace selection */
} /* namespace operation */
} /* namespace geneial */

