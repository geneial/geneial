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

	//Initialize the First Population
	_manager.replenishPopulation();

	//Do the barrel roll...
	while(!_stoppingCriterion->wasReached()){

		//TODO (bewo) implement genetic algorithm workflow here
		_manager.getPopulation().doAge();

		_manager.replenishPopulation();

	}

}


} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* BASEGENETICALGORITHM_HPP_ */
