/**
 *  Manager.cpp
 *  Created on: Dec 8, 2014
 *  Manages the population and most of the processes which modify the chromosomes
 *
 */

#include "Manager.h"
#include "Config.h"

namespace GeneticLibrary {
namespace Manager {

/**
 * Adds new Chromosomes to the population until a limit is reached (defined in config)
 * @param population the population wich should be replenished
 * @return a population wich contains a predefined number of chromosomes
 * @see config.h
 */
GeneticLibrary::Population::Population Manager::replenishPopulation (GeneticLibrary::Population::Population population){

	//if there are less chromosomes than required fill up:
	while (population.size() < defPopulationSize){

		//build new chromosome
		GeneticLibrary::Population::Chromosome::BasicChromosome newChromosome = GeneticLibrary::Population::Chromosome::BasicChromosome::BasicChromosome();

		//add at the end of vector
		population.getChromosomes().push_back(newChromosome);
	}
	return population;
}


/**
 * Selects the best Chromosome based on the fitness of each Chromosome in 'population'
 * @param population population containing at least one Chromosone
 * @return Chromosome with the highest fitness value
 */
GeneticLibrary::Population::Chromosome::BasicChromosome Manager::selectBestChromosome(GeneticLibrary::Population::Population population){
	GeneticLibrary::Population::Chromosome::BasicChromosome bestChromosome = population.getChromosomes().at(0);
	for (int i = 0; i <= population.size(); i++){
		if (population.getChromosomes().at(i) > bestChromosome.getFitness()){
			bestChromosome = population.getChromosomes().at(i);
		}
	}
	return bestChromosome;
}

} //namespace Manager
} //namespace Manager


