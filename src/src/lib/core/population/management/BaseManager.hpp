/**
 *  Manager.cpp
 *  Created on: Dec 8, 2014
 *  Manages the population and most of the processes which modify the chromosomes
 *
 */
#ifndef MANAGER_HPP_
#define MANAGER_HPP_

#include <core/population/management/BaseManager.h>
#include <iterator>

namespace GeneticLibrary {
namespace Population {
namespace Manager {

using namespace Chromosome;

template <typename FITNESS_TYPE>void BaseManager<FITNESS_TYPE>::replenishPopulation (){

	//if there are less chromosomes than required fill up:
	while (_population.getSize() < _populationSettings->getMaxChromosomes()){

		//build new chromosome
		BaseChromosome<FITNESS_TYPE> *newChromosome = _chromosomeFactory->createChromosome();

		//add at the end of vector
		_population.getChromosomes().push_back(newChromosome);
	}
}


template <typename FITNESS_TYPE>
void BaseManager<FITNESS_TYPE>::replacePopulation (typename Population<FITNESS_TYPE>::chromosome_container replacementPopulation){
	_population.getChromosomes().clear();
	std::copy(replacementPopulation.begin(),replacementPopulation.end(), std::back_inserter(_population.getChromosomes()));
}

template <typename FITNESS_TYPE>
unsigned int BaseManager<FITNESS_TYPE>::updateFitness(){
	//Iterate backwards over container (since new chromosomes are always inserted at the end)
	typename Population<FITNESS_TYPE>::chromosome_container::reverse_iterator rit =
			getPopulation().getChromosomes().rbegin();
	unsigned int num = 0;
	for (;rit != getPopulation().getChromosomes().rend()
					&& !(*rit)->hasFitness(); ++rit) {
		(*rit)->setFitness(_fitnessEvaluator->evaluate(*rit));
		num++;
	}
	return num;
}

template <typename FITNESS_TYPE>
Chromosome::BaseChromosome<FITNESS_TYPE> BaseManager<FITNESS_TYPE>::selectBestChromosome(Population<FITNESS_TYPE> population){
	/*
	 * TODO (bewo) use functors as argument for comparison and fitness class here
	 * {

	Chromosome::BaseChromosome<FITNESS_TYPE> bestChromosome = population.getChromosomes().at(0);
	for (int i = 0; i <= population.size(); i++){
		if (population.getChromosomes().at(i) > bestChromosome.getFitness()){
			bestChromosome = population.getChromosomes().at(i);
		}
	}
	return bestChromosome;

	*/
	return Chromosome::BaseChromosome<FITNESS_TYPE>(); //dummy
}

} /* namespace Manager */
} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* MANAGER_HPP_ */
