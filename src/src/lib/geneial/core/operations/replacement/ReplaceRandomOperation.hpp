/*
 * ReplaceRandomOperation.hpp
 *
 *  Created on: Dec 21, 2014
 *      Author: bewo
 */

#ifndef REPLACERANDOMOPERATION_HPP_
#define REPLACERANDOMOPERATION_HPP_

#include <algorithm>
#include <iterator>
#include <geneial/core/operations/replacement/BaseReplacementSettings.h>
#include <geneial/utility/random.h>

using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Utility;

namespace GeneticLibrary {
namespace Operation {
namespace Replacement {

template<typename FITNESS_TYPE>
unsigned int ReplaceRandomOperation<FITNESS_TYPE>::getAmountToReplace(
		const typename Population::Population<FITNESS_TYPE> &population,
		const typename Coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring) const{
	switch(this->getSettings()->getMode()){

		case BaseReplacementSettings::REPLACE_ALL_OFFSPRING:
		{
			return std::min(population.getChromosomes().size(),offspring.size());
		}
		break;

		case BaseReplacementSettings::REPLACE_FIXED_NUMBER: /*FALLTHROUGH*/
		default:
		{
			return std::min(population.getChromosomes().size(),
					(typename Population::Population<FITNESS_TYPE>::chromosome_map::size_type)
					this->getSettings()->getAmountToReplace());
		}
		break;
	}
}


template<typename FITNESS_TYPE>
void ReplaceRandomOperation<FITNESS_TYPE>::doReplace(
		typename Population::Population<FITNESS_TYPE> &population,
		typename Selection::BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &parents,
		typename Coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring,
		BaseManager<FITNESS_TYPE> &manager) {

	unsigned int numberToReplace = getAmountToReplace(population,offspring);

	//Remove random elements from the population w.r.t. elitism
	while(numberToReplace){
		//Ignore the very last chromosomes (elitism):
		const unsigned int max = population.getChromosomes().size() - this->getSettings()->getAmountElitism();

		//pick a random element to delete:
		const unsigned int rnd_advance = random::instance()->generateInt(0,max-1);

		//construct an iterator
		typename Population::Population<FITNESS_TYPE>::it advanced = population.getChromosomes().begin();
		std::advance(advanced,rnd_advance);

		//remove the element
		population.getChromosomes().erase(advanced);

		numberToReplace--;
	}

	//Insert all the offspring, parents are ignored here (since they are assumed to be already in the population)
	population.insertChromosomeContainer(offspring);

}

} /* namespace Replacement */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#endif /* REPLACERANDOMOPERATION_HPP_ */
