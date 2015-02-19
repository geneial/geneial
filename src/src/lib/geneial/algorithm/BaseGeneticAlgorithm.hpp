/*
 * BaseGeneticAlgorithm.hpp
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef BASEGENETICALGORITHM_HPP_
#define BASEGENETICALGORITHM_HPP_

#include <geneial/algorithm/BaseGeneticAlgorithm.h>
#include <iterator>
#include <set>

using namespace GeneticLibrary::Operation;

namespace GeneticLibrary {
namespace Algorithm {

template <typename FITNESS_TYPE>
void BaseGeneticAlgorithm<FITNESS_TYPE>::solve(){

	//TODO (bewo) use a Logger for output

	//TODO (bewo) scout for Memleaks using valgrind
	_wasStarted = true;


	//Initialize the first population candidate
	_manager.replenishPopulation();



//	std::cout << _manager.getPopulation() << std::endl;

	//Do a barrel roll...
	while(!_stoppingCriterion->wasReached(_manager)){

		_manager.getPopulation().doAge();

		//std::cout << _manager.getPopulation() << std::endl;

		//TODO (bewo) implement genetic algorithm workflow here {{{

		//Perform a selection of mating candidates based on the given strategy.
		typename Selection::BaseSelectionOperation<FITNESS_TYPE>::selection_result_set mating_pool; //<-- TODO (bewo) (use shared_ptr here?)
		mating_pool = _selectionOperation->doSelect(_manager.getPopulation(),_manager);

		/*typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container::iterator it =
				mating_pool.begin();

		std::cout << "MATING POOL:" << std::endl;
		for (;it != mating_pool.end(); ++it) {
			std::cout << **it << std::endl;
		}
		std::cout << "MATING POOL END"<< std::endl;*/

		typename Coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set offspring;
		offspring = _couplingOperation->doCopulate(mating_pool,_crossoverOperation,_manager);

		/*
		typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container::iterator it2 =
				offspring.begin();

		std::cout << "OFFSPRING POOL:" << std::endl;
		for (;it2 != offspring.end(); ++it2) {
			std::cout << **it2 << std::endl;
		}
		std::cout << "OFFSPRING POOL END"<< std::endl;
		*/

		offspring = _mutationOperation->doMutate(offspring,_manager);

		unsigned int removedDuplicates = _manager.getPopulation().removeDuplicates(offspring);

		_replacementOperation->doReplace(_manager.getPopulation(),mating_pool,offspring,_manager);



		//TODO (bewo) scaling?

		// }}}
	}
	_wasSolved = true;
}


} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* BASEGENETICALGORITHM_HPP_ */
