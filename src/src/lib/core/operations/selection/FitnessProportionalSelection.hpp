/*
 * FitnessProportionalSelection.h
 *
 *  Created on: Dec 11, 2014
 *      Author: bewo
 */

#ifndef FITNESSPROPORTIONALSELECTION_HPP_
#define FITNESSPROPORTIONALSELECTION_HPP_

#include <core/operations/selection/FitnessProportionalSelection.h>

using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Population::Chromosome;

namespace GeneticLibrary {
namespace Operation {
namespace Selection {


template <typename FITNESS_TYPE>
typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set FitnessProportionalSelection<FITNESS_TYPE>::doSelect(
		const Population::Population<FITNESS_TYPE> &population,
		BaseManager<FITNESS_TYPE> &manager){

		typedef std::multimap<FITNESS_TYPE, BaseChromosome<FITNESS_TYPE>* > map_type;

		//Calculate total sum of fitness
		FITNESS_TYPE sum(0);
		for (typename Population::Population<FITNESS_TYPE>::const_it it =
				population.getChromosomes().begin();
				it != population.getChromosomes().end(); ++it) {
			sum += (*it)->getFitness()->get();
		}

		assert(sum>0);

		//Create a multimap with fitness-proportional probability as key
		map_type sorted_multimap;
		for (typename Population::Population<FITNESS_TYPE>::const_it it =
				population.getChromosomes().begin();
				it != population.getChromosomes().end(); ++it) {
			FITNESS_TYPE prop_fitness = (*it)->getFitness()->get() / sum;
			sorted_multimap.insert(std::pair<FITNESS_TYPE,BaseChromosome<FITNESS_TYPE>* >(prop_fitness,*it));
		}

		SelectionSettings* settings = this->getSettings();

		typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set result;

		unsigned int left_select = settings->getNumberOfParents();
		//First, handle elitism, iterate backwards over the sorted multimap and select the best chromosomes.

		//TODO (bewo) OPTIMIZE: using sth like transform(i, j, back_inserter(v), select2nd<MapType::value_type>()); ???
		unsigned int elitism_to_select = settings->getElitismSuccessors();
		typename map_type::reverse_iterator crit = sorted_multimap.rbegin();
		for (;crit != sorted_multimap.rend() && elitism_to_select > 0; ++crit) {
			result.push_back(crit->second);
			elitism_to_select--;
		}
		//Strip the inserted part from the map
		sorted_multimap.erase ( crit.base(), sorted_multimap.end() );

		assert(result.size() == settings->getElitismSuccessors());
		left_select -= settings->getElitismSuccessors();
		assert(left_select <= sorted_multimap.size());


		//now select the remainder based on proportional probability.
		while(left_select > 0){
			//wrap around if necessary.
			typename map_type::reverse_iterator crit = sorted_multimap.rbegin();
			 while (crit != sorted_multimap.rend() && left_select > 0) {
				 BaseChromosome<FITNESS_TYPE>* chrom = crit->second;
				 double prob = (crit->first);
				 if(random::instance()->decision(prob)){
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
