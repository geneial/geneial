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

using namespace GeneticLibrary::Operation;

namespace GeneticLibrary {
namespace Algorithm {

template <typename FITNESS_TYPE>
void BaseGeneticAlgorithm<FITNESS_TYPE>::solve(){

	//TODO (bewo) use a Logger for output

	//TODO (bewo) scout for Memleaks using valgrind

	std::cout << "Initial Replenishing" << std::endl;

	//Initialize the first population candidate
	_manager.replenishPopulation();

	std::cout << _manager.getPopulation().getAge() << std::endl;


	std::cout << "Initial Population" << std::endl;
	std::cout << _manager.getPopulation() << std::endl;

	std::cout << "Entering Main Loop" << std::endl;

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


		offspring = _mutationOperation->doMutate(offspring);

		typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container::iterator offspring_it =
				offspring.begin();

		for (; offspring_it != offspring.end();
				++offspring_it) {
			typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container::iterator offspring_it2 =
					offspring.begin();
			for (; offspring_it2 != offspring.end();) {
				if (offspring_it != offspring_it2 && (*offspring_it)->equals(*offspring_it2)) {
					offspring_it2 = offspring.erase(offspring_it2);
				} else {
					++offspring_it2;
				}
			}
		}

		typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_map::iterator pop_it =
		_manager.getPopulation().getChromosomes().begin();


		//Remove offspring duplicates
		//TODO (bewo): This is very inefficient.
		for (; pop_it != _manager.getPopulation().getChromosomes().end();
				++pop_it) {
			typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container::iterator it2 =
					offspring.begin();
			for (; it2 != offspring.end();) {
				if (pop_it->second->equals(*it2)) {
					it2 = offspring.erase(it2);
				} else {
					++it2;
				}
			}
		}


		_replacementOperation->doReplace(_manager.getPopulation(),mating_pool,offspring,_manager);

		//TODO (bewo) mutation

		//TODO (bewo) scaling

		//std::cout << "Age: " << _manager.getPopulation().getAge() << std::endl;

		// }}}

	}

	std::cout << "Main Loop Done" << std::endl;

	std::cout << "Final Population" << std::endl;
	std::cout << _manager.getPopulation() << std::endl;

}


} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* BASEGENETICALGORITHM_HPP_ */
