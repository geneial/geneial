/*
 * BasicChromome.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#include <core/population/chromosome/BaseChromosome.h>

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {


template <class FITNESS_TYPE> Fitness<FITNESS_TYPE>* BaseChromosome<FITNESS_TYPE>::getFitness() const{
	return _fitness;
}

template <class FITNESS_TYPE> void BaseChromosome<FITNESS_TYPE>::setFitness(Fitness<FITNESS_TYPE> * const fit){
	_fitness = fit;
}

template <class FITNESS_TYPE> unsigned int BaseChromosome<FITNESS_TYPE>::doAge(){
	return ++_age;
}

template <class FITNESS_TYPE> void BaseChromosome<FITNESS_TYPE>::setAge(unsigned int age){
	_age = age;
}

template <class FITNESS_TYPE> unsigned int BaseChromosome<FITNESS_TYPE>::getAge() const{
	return _age;
}


} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */
