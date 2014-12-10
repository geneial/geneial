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
#include <core/population/PopulationSettings.h>
#include <core/population/builder/BaseChromosomeFactory.h>

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

	Chromosome::BaseChromosomeFactory<FITNESS_TYPE> *_chromosomeFactory;

	PopulationSettings *_populationSettings;

public:
	BaseManager<FITNESS_TYPE>(
		PopulationSettings *populationSettings,
		Chromosome::BaseChromosomeFactory<FITNESS_TYPE> *chromosomeFactory
		):
	_populationSettings(populationSettings),
	_chromosomeFactory(chromosomeFactory)
	{
		assert(_chromosomeFactory != NULL);
		//assert(_populationSettings != NULL); TODO (bewo)
	};



	/**
	 * Selects the best Chromosome based on the fitness of each Chromosome in 'population'
	 * @param population population containing at least one Chromosone
	 * @return Chromosome with the highest fitness value
	 */

	Chromosome::BaseChromosome<FITNESS_TYPE> selectBestChromosome(Population<FITNESS_TYPE> population);

	/**
	 * Adds new chromosomes to the population until a limit is reached
	 * @return a population which contains a predefined number of chromosomes
	 */
	Population<FITNESS_TYPE> replenishPopulation ();

	const Population<FITNESS_TYPE>& getPopulation() const {
		return _population;
	}

	const Chromosome::BaseChromosomeFactory<FITNESS_TYPE>*& getChromosomeFactory() const {
		return _chromosomeFactory;
	}

	void setChromosomeFactory(
			const Chromosome::BaseChromosomeFactory<FITNESS_TYPE>*& chromosomeFactory) {
		_chromosomeFactory = chromosomeFactory;
	}

	const PopulationSettings*& getPopulationSettings() const {
		return _populationSettings;
	}

	void setPopulationSettings(
			const PopulationSettings*& populationSettings) {
		_populationSettings = populationSettings;
	}
};

} /* namespace Manager */
} /* namespace Population */
} /* namespace GeneticLibrary */


#include "BaseManager.hpp"

#endif /* MANAGER_H_ */
