/*
 * Population.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <core/population/Population.h>

namespace GeneticLibrary {
namespace Population {

/**
 * Creates a vector containing ONE chromosome, the population needs to be build with replenishPopulation
 * @see replenishPopulation
 */
template <typename FITNESS_TYPE>
Population<FITNESS_TYPE>::Population(): _age(0) {
	//put one Chromosome into the population to create a starting point
	//TODO (bewo)
	//chromosomes.push_back(Chromosome::BasicChromosome());
}

template <typename FITNESS_TYPE>
Population<FITNESS_TYPE>::~Population() {
}

template <typename FITNESS_TYPE>
void Population<FITNESS_TYPE>::print(std::ostream& os) const {
	os << "Population: ";
	//TODO (bewo)
}

/**
 * Returns a Vector of Chromosomes, each Chromosome contains a set of Values and a fitness value
 * @return A vector of chromosomes
 */
template <typename FITNESS_TYPE>
std::vector<Chromosome::BaseChromosome<FITNESS_TYPE> > Population<FITNESS_TYPE>::getChromosomes(){
	return this->chromosomes;
}


/**
 *
 * @return The amount of chomosones wich are part of the population.
 */
template <typename FITNESS_TYPE>
int Population<FITNESS_TYPE>::size() const{
	return this-> chromosomes.size();
}


template <typename FITNESS_TYPE>
unsigned int Population<FITNESS_TYPE>::getAge() const{
	return _age;
}

template <typename FITNESS_TYPE>
void Population<FITNESS_TYPE>::setAge(unsigned int age){
	_age = age;
}

/**
 * Increments the age of the current population by 1
 */
template <typename FITNESS_TYPE>
void Population<FITNESS_TYPE>::doAge(){
	++_age;
}

} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* POPULATION_HPP_ */
