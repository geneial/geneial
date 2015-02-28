/*
 * ReplaceWorstOperation.h
 *
 *  Created on: Dec 21, 2014
 *      Author: bewo
 */

#ifndef REPLACEWORSTOPERATION_HPP_
#define REPLACEWORSTOPERATION_HPP_

#include <geneial/core/operations/replacement/BaseReplacementSettings.h>

#include <algorithm>
#include <iterator>


namespace GeneticLibrary {
namespace Operation {
namespace Replacement {

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Manager;

template<typename FITNESS_TYPE>
unsigned int ReplaceWorstOperation<FITNESS_TYPE>::getAmountToReplace(
		const Population<FITNESS_TYPE> &population,
		const typename Coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring) const{
	switch(this->getSettings()->getMode()){

		case BaseReplacementSettings::REPLACE_ALL_OFFSPRING:
		{
			return std::min(population.getFitnessMap().size()-this->getSettings()->getAmountElitism()-1,offspring.size());
		}
		break;

		case BaseReplacementSettings::REPLACE_FIXED_NUMBER: /*FALLTHROUGH*/
		default:
		{
			return std::min(population.getFitnessMap().size()-1,
					(typename Population<FITNESS_TYPE>::fitness_map::size_type)
					this->getSettings()->getAmountToReplace());
		}
		break;
	}
}


template<typename FITNESS_TYPE>
void ReplaceWorstOperation<FITNESS_TYPE>::doReplace(
		Population<FITNESS_TYPE> &population,
		typename Selection::BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &parents,
		typename Coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring,
		BaseManager<FITNESS_TYPE> &manager) {

	unsigned int numberToReplace = getAmountToReplace(population,offspring); //this also takes care of elitism!

	//remove the worst n chromosomes to replace (assuming worst is at the very beginning)
	typename Population<FITNESS_TYPE>::fitnessmap_const_it advanced = population.getFitnessMap().begin();
	std::advance(advanced,numberToReplace);
	typename Population<FITNESS_TYPE>::chromosome_container toRemove;
	//TODO (bewo) copy?
	for(typename Population<FITNESS_TYPE>::fitnessmap_const_it it = population.getFitnessMap().begin();
			it != advanced; it++ ){
		toRemove.push_back(it->second);
	}
	population.removeChromosomeContainer(toRemove);

	//Insert all the offspring, parents are ignored here (since they are assumed to be already in the population)
	population.insertChromosomeContainer(offspring);

}

} /* namespace Replacement */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#endif /* REPLACEWORSTOPERATION_HPP_ */
