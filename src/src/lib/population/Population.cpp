/*
 * Population.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#include "Population.h"
#include "Config.h"
#include <vector>

namespace GeneticLibrary {
namespace Population {

Population::Population() {
	//put one Chromosome into the population to create a starting point
	chromosomes.push_back(GeneticLibrary::Population::Chromosome::BasicChromosome());
}

Population::~Population() {
}

void Population::print(std::ostream& os) const {
	os << "Population: ";
}

std::vector<GeneticLibrary::Population::Chromosome::BasicChromosome> Population::getChromosomes(){
	return this->chromosomes;
}



int Population::size(){
	return this-> chromosomes.size();
}

} /* namespace Population */
} /* namespace GeneticLibrary */
