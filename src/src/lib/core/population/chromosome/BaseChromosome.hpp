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


template <class FT> BaseChromosome<FT>::BaseChromosome(): _age(CHROMOSOME_AGE_UNITIALIZED) {

}
template <class FT> BaseChromosome<FT>::~BaseChromosome() {
}

template <class FT> Fitness<FT> BaseChromosome<FT>::getFitness() const{
	return _fitness;
}

template <class FT> void BaseChromosome<FT>::setFitness(const Fitness<FT> fit){
	_fitness = fit;
}


template <class FT> unsigned int BaseChromosome<FT>::doAge(){
	return ++_age;
}

template <class FT> void BaseChromosome<FT>::setAge(unsigned int age){
	_age = age;
}

template <class FT> unsigned int BaseChromosome<FT>::getAge() const{
	return _age;
}


} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */
