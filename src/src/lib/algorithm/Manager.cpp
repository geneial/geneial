/*
 * Manager.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: lukas
 */

#include "Manager.h"
#include "Config.h"

namespace GeneticLibrary {
namespace Manager {

GeneticLibrary::Population::Population replenishPopulation (GeneticLibrary::Population::Population population){

	//if there are less chromosomes than required fill up:
	while (population.size() < defPoplationSize){

		//build new chromosome
		GeneticLibrary::Population::Chromosome::BasicChromosome newChromosome = GeneticLibrary::Population::Chromosome::BasicChromosome::BasicChromosome();

		//add at the end of vector
		population.getChromosomes().push_back(newChromosome);
	}
	return population;
}

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


