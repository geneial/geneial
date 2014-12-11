/*
 * BaseGeneticAlgorithm.hpp
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef BASEGENETICALGORITHM_HPP_
#define BASEGENETICALGORITHM_HPP_

#include <algorithm/BaseGeneticAlgorithm.h>

#include <algorithm> //std::set_intersection

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
	//Initial fitness
	_manager.updateFitness();

	std::cout << _manager.getPopulation().getAge() << std::endl;


	std::cout << "Initial Population" << std::endl;
	std::cout << _manager.getPopulation() << std::endl;

	std::cout << "Entering Main Loop" << std::endl;

	//Do a barrel roll...
	while(!_stoppingCriterion->wasReached(_manager)){

		//std::cout << _manager.getPopulation() << std::endl;

		//TODO (bewo) implement genetic algorithm workflow here {{{

		typename Selection::BaseSelectionOperation<FITNESS_TYPE>::selection_result_set selectionResult;
		selectionResult = _selectionOperation->doSelect(_manager.getPopulation(),_manager);

		//For testing purposes just replace the current population by the selection result
		_manager.replacePopulation(selectionResult);

		//TODO (bewo) crossover
		//TODO (bewo) mutation
		//TODO (bewo) scaling



		_manager.getPopulation().doAge();

		// }}}

		_manager.replenishPopulation();

		_manager.updateFitness();

	}

	std::cout << "Main Loop Done" << std::endl;

	std::cout << "Final Population" << std::endl;
	std::cout << _manager.getPopulation() << std::endl;

}


} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* BASEGENETICALGORITHM_HPP_ */