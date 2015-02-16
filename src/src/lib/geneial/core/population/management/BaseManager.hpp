/**
 *  Manager.cpp
 *  Created on: Dec 8, 2014
 *  Manages the population and most of the processes which modify the chromosomes
 *
 */
#ifndef MANAGER_HPP_
#define MANAGER_HPP_

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/core/population/Population.h>
#include <iterator>

namespace GeneticLibrary {
namespace Population {
namespace Manager {

using namespace Chromosome;

template <typename FITNESS_TYPE>
void BaseManager<FITNESS_TYPE>::replenishPopulation (){

	//if there are less chromosomes than required fill up:
	while (_population.getSize() < _populationSettings->getMaxChromosomes()){

		//build new chromosome
		typename BaseChromosome<FITNESS_TYPE>::ptr newChromosome = _chromosomeFactory->createChromosome();

		FITNESS_TYPE fitness = newChromosome->getFitness()->get();
		typename Population<FITNESS_TYPE>::container_value value(fitness,newChromosome);

		//add at the end of vector
		_population.getChromosomes().insert(value);
	}
}


template <typename FITNESS_TYPE>
void BaseManager<FITNESS_TYPE>::replacePopulation (typename Population<FITNESS_TYPE>::chromosome_container replacementPopulation){
	_population.getChromosomes().clear();
	_population.insertChromosomeContainer(replacementPopulation);
}

//template <typename FITNESS_TYPE>
//typename BaseChromosome<FITNESS_TYPE>::ptr BaseManager<FITNESS_TYPE>::getHighestFitnessChromosome() const{
//	return this->_population.getChromosomes().rbegin()->second;
//}


template <typename FITNESS_TYPE>
FITNESS_TYPE BaseManager<FITNESS_TYPE>::getHighestFitness()  const{
	return this->_population.getChromosomes().rbegin()->first;
}

template <typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr BaseManager<FITNESS_TYPE>::getLowestFitnessChromosome() const{
	return this->_population.getChromosomes().begin()->second;
}

template <typename FITNESS_TYPE>
FITNESS_TYPE BaseManager<FITNESS_TYPE>::getLowestFitness() const{
	return this->_population.getChromosomes().begin()->first;
}

} /* namespace Manager */
} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* MANAGER_HPP_ */
