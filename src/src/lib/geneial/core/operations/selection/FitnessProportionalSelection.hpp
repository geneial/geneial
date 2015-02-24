/*
 * FitnessProportionalSelection.h
 *
 *  Created on: Dec 11, 2014
 *      Author: bewo
 */

#ifndef FITNESSPROPORTIONALSELECTION_HPP_
#define FITNESSPROPORTIONALSELECTION_HPP_

#include <geneial/core/operations/selection/FitnessProportionalSelection.h>
#include <geneial/utility/Random.h>

#include <cassert>


namespace GeneticLibrary {
namespace Operation {
namespace Selection {

using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Utility;

template <typename FITNESS_TYPE>
typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set FitnessProportionalSelection<FITNESS_TYPE>::doSelect(
		const Population<FITNESS_TYPE> &population,
		BaseManager<FITNESS_TYPE> &manager){

		//shorthands for type mess
		typedef std::multimap<FITNESS_TYPE, typename BaseChromosome<FITNESS_TYPE>::ptr > map_type;
		typedef typename BaseChromosome<FITNESS_TYPE>::ptr chrom_ptr_type;
		typedef typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set result_set;
		typedef typename Population<FITNESS_TYPE>::const_it const_pop_itr;

		result_set result;


		//Calculate total sum of fitness
		FITNESS_TYPE sum(0);
		for (const_pop_itr it =	population.getChromosomes().begin();
				it != population.getChromosomes().end(); ++it) {
			sum += (it->second)->getFitness()->get();
		}
		//TODO (bewo) maybe use a fancy-ass functor instead?

		assert(sum>0);

		//Create a multimap with fitness-proportional probability as key
		map_type sorted_multimap;
		for (const_pop_itr it = population.getChromosomes().begin();
				it != population.getChromosomes().end(); ++it) {
			FITNESS_TYPE prop_fitness = (it->second)->getFitness()->get() / sum;
			sorted_multimap.insert(std::pair<FITNESS_TYPE, chrom_ptr_type >(prop_fitness,it->second));
		}

		unsigned int left_select = _settings->getNumberOfParents();
		//First, handle elitism, iterate backwards over the sorted multimap and select the best chromosomes.

		//TODO (bewo) OPTIMIZE: using sth like transform(i, j, back_inserter(v), select2nd<MapType::value_type>()); instead ???
		unsigned int elitism_to_select = _settings->getNumberSelectBest();
		typename map_type::reverse_iterator crit = sorted_multimap.rbegin();
		for (;crit != sorted_multimap.rend() && elitism_to_select > 0; ++crit) {
			result.push_back(crit->second);
			elitism_to_select--;
		}
		//Strip the inserted part from the map
		sorted_multimap.erase ( crit.base(), sorted_multimap.end() );

		assert(result.size() == _settings->getNumberSelectBest());
		left_select -= _settings->getNumberSelectBest();
		assert(left_select <= sorted_multimap.size());


		//now select the remainder based on proportional probability.
		while(left_select > 0){
			//wrap around if necessary.
			typename map_type::reverse_iterator crit = sorted_multimap.rbegin();
			 while (crit != sorted_multimap.rend() && left_select > 0) {
				 chrom_ptr_type chrom = crit->second;
				 double prob = (crit->first);
				 if(Random::instance()->decision(prob)){
					 //Use it.
					 result.push_back(chrom);
					 left_select--;
					 sorted_multimap.erase((++crit).base()); //erase requires normal iterator,
				 }else{
					 //Skip this chromosome for now.
					 ++crit;
				 }
			 }
			 assert(left_select <= sorted_multimap.size());
		}
		return result;
}


} /* namespace Selection */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#endif /* FITNESSPROPORTIONALSELECTION_HPP_ */
