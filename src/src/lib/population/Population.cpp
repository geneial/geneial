/*
 * Population.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#include "Population.h"

namespace GeneticLibrary {
namespace Population {

Population::Population() {
	//TODO: call factory to generate Popuation here
}

Population::~Population() {
}

void Population::print(std::ostream& os) const {
	os << "Population: ";
}

std::vector<GeneticLibrary::Population::Chromosome::BasicChromosome> Population::getPopulation(){
	return this->chromosomes;
}

void Population::generatePopulation(){
//TODO: Call Factory to generate/ fill up Population here
}

} /* namespace Population */
} /* namespace GeneticLibrary */
