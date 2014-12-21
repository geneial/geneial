/*
 * ReplaceWorstOperation.h
 *
 *  Created on: Dec 21, 2014
 *      Author: bewo
 */

#ifndef REPLACEWORSTOPERATION_HPP_
#define REPLACEWORSTOPERATION_HPP

#include <algorithm>
#include <iterator>
#include <geneial/core/operations/replacement/BaseReplacementSettings.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Operation {
namespace Replacement {

template<typename FITNESS_TYPE>
unsigned int ReplaceWorstOperation<FITNESS_TYPE>::getAmountToReplace(
		const typename Population::Population<FITNESS_TYPE> &population,
		const typename Coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring) const{
	switch(this->getSettings()->getMode()){

		case BaseReplacementSettings::REPLACE_ALL_OFFSPRING:
		{
			return std::min(population.getChromosomes().size()-this->getSettings()->getAmountElitism(),offspring.size());
		}
		break;

		case BaseReplacementSettings::REPLACE_FIXED_NUMBER: /*FALLTHROUGH*/
		default:
		{
			return std::min(population.getChromosomes().size,
					(typename Population::Population<FITNESS_TYPE>::chromosome_map::size_type)
					this->getSettings()->getAmountToReplace());
		}
		break;
	}
}


template<typename FITNESS_TYPE>
void ReplaceWorstOperation<FITNESS_TYPE>::doReplace(
		typename Population::Population<FITNESS_TYPE> &population,
		typename Selection::BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &parents,
		typename Coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring,
		BaseManager<FITNESS_TYPE> &manager) {

	unsigned int numberToReplace = getAmountToReplace(population,offspring); //this also takes care of elitism!

	//remove the worst n chromosomes to replace (assuming worst is at the very beginning)
	typename Population::Population<FITNESS_TYPE>::it advanced = population.getChromosomes().begin();
	std::advance(advanced,numberToReplace);
	population.getChromosomes().erase(population.getChromosomes().begin(),advanced);

	//Insert all the offspring, parents are ignored here (since they are assumed to be already in the population)
	population.insertChromosomeContainer(offspring);

}

} /* namespace Replacement */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#endif /* REPLACEWORSTOPERATION_HPP_ */
