/*
 * BasicChromome.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef BASE_CHROMOSOME_HPP_
#define BASE_CHROMOSOME_HPP_

#include <geneial/core/population/chromosome/BaseChromosome.h>

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {


template <typename FITNESS_TYPE>
const typename Fitness<FITNESS_TYPE>::ptr BaseChromosome<FITNESS_TYPE>::getFitness(){

	if(!hasFitnessEvaluator()){
		//No fitness evaluator -> undefined, probably null but no guarantee.
		return _fitness;
	}

	if(hasFitnessEvaluator() && !hasFitness()){
		//if we have a fitness evaluator assigned to the chromosome, but no fitness was calculated yet, do so.
		_fitness = _fitnessEvaluator->evaluate(this->getPtr());
	}

	return _fitness;

}


template <typename FITNESS_TYPE>
const typename Fitness<FITNESS_TYPE>::ptr BaseChromosome<FITNESS_TYPE>::getFitness() const{
	return _fitness;
}

template <typename FITNESS_TYPE>
void BaseChromosome<FITNESS_TYPE>::setFitness(const typename Fitness<FITNESS_TYPE>::ptr& fit){
	_fitness = fit;
}

template <typename FITNESS_TYPE>
const typename FitnessEvaluator<FITNESS_TYPE>::ptr BaseChromosome<FITNESS_TYPE>::getFitnessEvaluator() const{
	return _fitnessEvaluator;
}

template <typename FITNESS_TYPE>
void BaseChromosome<FITNESS_TYPE>::setFitnessEvaluator(
		const typename FitnessEvaluator<FITNESS_TYPE>::ptr& fitnessEvaluator) {
	_fitnessEvaluator = fitnessEvaluator;
}

template <typename FITNESS_TYPE> unsigned int BaseChromosome<FITNESS_TYPE>::doAge(){
	return ++_age;
}

template <typename FITNESS_TYPE> void BaseChromosome<FITNESS_TYPE>::setAge(unsigned int age){
	_age = age;
}

template <typename FITNESS_TYPE> unsigned int BaseChromosome<FITNESS_TYPE>::getAge() const{
	return _age;
}


} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */


#endif /* BASE_CHROMOSOME_HPP_ */
