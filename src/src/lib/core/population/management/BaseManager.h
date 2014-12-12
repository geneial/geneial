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
#include <core/fitness/FitnessEvaluator.h>
#include <assert.h>

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

	FitnessEvaluator<FITNESS_TYPE> *_fitnessEvaluator;

public:
	BaseManager<FITNESS_TYPE>(
		PopulationSettings *populationSettings,
		Chromosome::BaseChromosomeFactory<FITNESS_TYPE> *chromosomeFactory,
		FitnessEvaluator<FITNESS_TYPE> *fitnessEvaluator
		):
	_population(),
	_chromosomeFactory(chromosomeFactory),
	_populationSettings(populationSettings),
	_fitnessEvaluator(fitnessEvaluator)
	{
		assert(_chromosomeFactory != NULL);
		assert(_populationSettings != NULL);
		assert(_fitnessEvaluator != NULL);
	};


	void replacePopulation (typename Population<FITNESS_TYPE>::chromosome_container replacementPopulation);

	unsigned int updateFitness();




	/**
	 * Selects the best Chromosome based on the fitness of each Chromosome in 'population'
	 * @param population population containing at least one Chromosone
	 * @return Chromosome with the highest fitness value
	 */

	Chromosome::BaseChromosome<FITNESS_TYPE> selectBestChromosome(Population<FITNESS_TYPE> population);

	/**
	 * Adds new chromosomes to the population until a limit is reached
	 */
	void replenishPopulation ();

	Population<FITNESS_TYPE>& getPopulation() {
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


#include <core/population/management/BaseManager.hpp>

#endif /* MANAGER_H_ */
