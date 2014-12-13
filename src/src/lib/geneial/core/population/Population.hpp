/*
 * Population.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <geneial/core/population/Population.h>

namespace GeneticLibrary {
namespace Population {

/**
 * Creates a vector containing ONE chromosome, the population needs to be build with replenishPopulation
 * @see replenishPopulation
 */
template <typename FITNESS_TYPE>
Population<FITNESS_TYPE>::Population(): _age(0) {}

template <typename FITNESS_TYPE>
Population<FITNESS_TYPE>::~Population() {
}

template <typename FITNESS_TYPE>
void Population<FITNESS_TYPE>::print(std::ostream& os) const {
	os << "Population: Age (" << _age << "), #Chromosomes ("<< _chromosomes.size() <<"):" << std::endl;
	os << "  Chromosomes" << std::endl;

	//TODO (bewo) maybe use outstream iterator instead here.
	for(typename chromosome_container::const_iterator chrom_it = _chromosomes.begin(); chrom_it != _chromosomes.end(); ++chrom_it) {
		os << **chrom_it;
	}
}


/**
 *
 * @return The amount of chomosones wich are part of the population.
 */
template <typename FITNESS_TYPE>
typename Population<FITNESS_TYPE>::population_size Population<FITNESS_TYPE>::getSize() const{
	return _chromosomes.size();
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
template<typename FITNESS_TYPE>
void Population<FITNESS_TYPE>::doAge() {
	for (typename chromosome_container::iterator chrom_it =
			_chromosomes.begin(); chrom_it != _chromosomes.end(); ++chrom_it) {
		(*chrom_it)->doAge();
	}
	++_age;
}

} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* POPULATION_HPP_ */
