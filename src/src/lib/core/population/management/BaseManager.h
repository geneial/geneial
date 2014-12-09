/*
 * Manager.h
 *
 *  Created on: Dec 8, 2014
 *      Author: lukas
 */


#ifndef MANAGER_H_
#define MANAGER_H_

#include <core/population/chromosome/BaseChromosome.h>
#include <core/population/Population.h>

namespace GeneticLibrary {
namespace Population {
namespace Manager {
/**
 * Manages the population and most of the processes which modify the chromosomes
 */
template <typename FITNESS_TYPE>
class BaseManager{
private:
	Population<FITNESS_TYPE> _population;
public:
	/**
	 * Adds new chromosomes to the population until a limit is reached
	 * @param population the population which should be replenished
	 * @return a population which contains a predefined number of chromosomes
	 * @see config.h
	 */
	Chromosome::BaseChromosome<FITNESS_TYPE> selectBestChromosome(Population<FITNESS_TYPE> population);

	/**
	 * Selects the best Chromosome based on the fitness of each Chromosome in 'population'
	 * @param population population containing at least one Chromosone
	 * @return Chromosome with the highest fitness value
	 */
	Population<FITNESS_TYPE> replenishPopulation (Population<FITNESS_TYPE> population);
};

} /* namespace Manager */
} /* namespace Population */
} /* namespace GeneticLibrary */


#include "BaseManager.hpp"

#endif /* MANAGER_H_ */
