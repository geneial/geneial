/*
 * Manager.h
 *
 *  Created on: Dec 8, 2014
 *      Author: lukas
 */


#ifndef MANAGER_H_
#define MANAGER_H_

#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/core/population/Population.h>
#include <geneial/core/population/PopulationSettings.h>
#include <geneial/core/population/builder/BaseChromosomeFactory.h>
#include <assert.h>

namespace GeneticLibrary {
namespace Population {
namespace Manager {
/**
 * Manages the population and most of the processes which modify the chromosomes
 */
template <typename FITNESS_TYPE>
class BaseManager{


public:
	BaseManager<FITNESS_TYPE>(
		PopulationSettings *populationSettings,
		Chromosome::BaseChromosomeFactory<FITNESS_TYPE> *chromosomeFactory
		):
	_population(),
	_chromosomeFactory(chromosomeFactory),
	_populationSettings(populationSettings)
	{
		assert(_chromosomeFactory != NULL);
		assert(_populationSettings != NULL);
	};


	void replacePopulation (typename Population<FITNESS_TYPE>::chromosome_container replacementPopulation);

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
private:
	Population<FITNESS_TYPE> _population;

	Chromosome::BaseChromosomeFactory<FITNESS_TYPE> *_chromosomeFactory;

	PopulationSettings *_populationSettings;

};

} /* namespace Manager */
} /* namespace Population */
} /* namespace GeneticLibrary */


#include <geneial/core/population/management/BaseManager.hpp>

#endif /* MANAGER_H_ */
