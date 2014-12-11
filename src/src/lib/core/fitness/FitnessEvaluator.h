/*
 * FitnessEvaluator.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef FITNESSEVALUATOR_H_
#define FITNESSEVALUATOR_H_

#include <core/population/chromosome/BaseChromosome.h>

using namespace GeneticLibrary::Population::Chromosome;

namespace GeneticLibrary{

template <typename FITNESS_TYPE>
class FitnessEvaluator {
public:
	FitnessEvaluator() {};
	virtual ~FitnessEvaluator(){}
	virtual typename Fitness<FITNESS_TYPE>::ptr evaluate(const typename BaseChromosome<FITNESS_TYPE>::ptr chromosome) const= 0;
};

} /* namespace GeneticLibrary */

#endif /* FITNESSEVALUATOR_H_ */
