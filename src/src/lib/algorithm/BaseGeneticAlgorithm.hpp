/*
 * BaseGeneticAlgorithm.hpp
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef BASEGENETICALGORITHM_HPP_
#define BASEGENETICALGORITHM_HPP_


namespace GeneticLibrary {
namespace Algorithm {

template <typename FITNESS_TYPE>
void BaseGeneticAlgorithm<FITNESS_TYPE>::solve(){


	std::cout << "Initial Replenishing" << std::endl;

	//Initialize the First Population
	_manager.replenishPopulation();


	std::cout << _manager.getPopulation().getAge() << std::endl;

	std::cout << "Main Loop" << std::endl;

	//Do the barrel roll...
	while(!_stoppingCriterion->wasReached(_manager)){

		//TODO (bewo) implement genetic algorithm workflow here
		_manager.getPopulation().doAge();

		std::cout << "Age:" << _manager.getPopulation().getAge() << std::endl;

		_manager.replenishPopulation();

	}

}


} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* BASEGENETICALGORITHM_HPP_ */
