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
void BaseManager<FITNESS_TYPE>::replenishPopulation ()
{
	//if there are less chromosomes than required fill up:
	while (_population.getSize() < _populationSettings->getMaxChromosomes()){
		//build new chromosome
		typename BaseChromosome<FITNESS_TYPE>::ptr newChromosome = _chromosomeFactory->createChromosome();
		_population.insertChromosome(newChromosome);
	}
}

template <typename FITNESS_TYPE>
void BaseManager<FITNESS_TYPE>::replacePopulation (typename Population<FITNESS_TYPE>::chromosome_container replacementPopulation)
{
	_population.replacePopulation(replacementPopulation);
}

template <typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr BaseManager<FITNESS_TYPE>::getHighestFitnessChromosome() const
{
	return _population.getFitnessMap().rbegin()->second;
}


template <typename FITNESS_TYPE>
FITNESS_TYPE BaseManager<FITNESS_TYPE>::getHighestFitness() const
{
	return _population.getFitnessMap().rbegin()->first;
}

template <typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr BaseManager<FITNESS_TYPE>::getLowestFitnessChromosome() const
{
	return _population.getFitnessMap().begin()->second;
}

template <typename FITNESS_TYPE>
FITNESS_TYPE BaseManager<FITNESS_TYPE>::getLowestFitness() const
{
	return _population.getFitnessMap().begin()->first;
}

} /* namespace Manager */
} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* MANAGER_HPP_ */
