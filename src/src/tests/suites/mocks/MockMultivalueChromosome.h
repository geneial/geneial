#ifndef MOCKMULTIVALUECHROMOSOME_H_
#define MOCKMULTIVALUECHROMOSOME_H_

#include "boost/shared_ptr.hpp"
#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/fitness/FitnessEvaluator.h>
#include "MockFitnessEvaluator.h"

template <typename VALUE_TYPE,typename FITNESS_TYPE>
inline typename BaseChromosome<FITNESS_TYPE>::ptr baseCastMVC(typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr chromosome){
	typename BaseChromosome<FITNESS_TYPE>::ptr result = boost::dynamic_pointer_cast<BaseChromosome<FITNESS_TYPE> >(chromosome);
	return result;
}

MultiValueChromosome<int,double>::const_ptr getMockIntMVC(std::vector<int> values,
		FitnessEvaluator<double>::ptr ptr_evaluator = boost::shared_ptr<FitnessEvaluator<double> >()){
	GeneticLibrary::FitnessEvaluator<double>::ptr evaluator = ptr_evaluator;
	if(!evaluator){
		evaluator = FitnessEvaluator<double>::ptr(new MockDoubleFitnessEvaluator());
	}
	MultiValueChromosome<int,double>::ptr new_chromosome(new MultiValueChromosome<int,double>(evaluator));
	new_chromosome->getContainer().reserve(values.size());
	new_chromosome->getContainer().swap(values);
	return new_chromosome;
}


MultiValueChromosome<double,double>::const_ptr getMockDoubleMVC(std::vector<double> values,
		FitnessEvaluator<double>::ptr ptr_evaluator = boost::shared_ptr<FitnessEvaluator<double> >()){
	GeneticLibrary::FitnessEvaluator<double>::ptr evaluator = ptr_evaluator;
	if(!evaluator){
		evaluator = FitnessEvaluator<double>::ptr(new MockDoubleFitnessEvaluator());
	}

	MultiValueChromosome<double,double>::ptr new_chromosome(new MultiValueChromosome<double,double>(evaluator));
	new_chromosome->getContainer().reserve(values.size());
	new_chromosome->getContainer().swap(values);
	return new_chromosome;
}

#endif /* MOCKMULTIVALUECHROMOSOME_H_ */
