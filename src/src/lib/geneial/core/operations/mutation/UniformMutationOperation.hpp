/*
 * UniformMutationOperation.hpp
 *
 *  Created on: Dec 30, 2014
 *      Author: lukas
 */

#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_UNIFORMMUTATIONOPERATION_HPP_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_UNIFORMMUTATIONOPERATION_HPP_

#include <geneial/core/operations/mutation/UniformMutationOperation.h>
#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/population/builder/BuilderSettings.h>
//#include <geneial/core/operations/mutation/BaseMutationOperation.h>

using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Operation::Mutation;

namespace GeneticLibrary {
namespace Operation {
namespace Mutation {

template<typename VALUE_TYPE, typename FITNESS_TYPE>
/*
 *  Returns a new chromosome which is a partially mutated version of the old one.
 *
 *  */
typename BaseMutationOperation<FITNESS_TYPE>::mutation_result_set UniformMutationOperation<VALUE_TYPE, FITNESS_TYPE>::doMutate(typename BaseChromosome<FITNESS_TYPE>::ptr mutant){

	typedef typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::value_container value_container;
	typedef typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr mvc_ptr;

	typename Mutation::BaseMutationOperation<FITNESS_TYPE>::mutation_result_set resultset;

	//casting mutant as MVC
	mvc_ptr mvc_mutant
			= boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> >(mutant);
	assert(mvc_mutant);

	//creating a new MVC (to keep things reversible)
	mvc_ptr mutation_result =
				boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> >(
						this->getBuilderFactory()->createChromosome()
				);
	assert(mutation_result);


	//getting values
	value_container &mutant_container = mvc_mutant->getContainer();
	value_container &result_container = mutation_result->getContainer();
	result_container.clear();

	typename value_container::iterator mutant_it = mutant_container.begin();

	double mutate_chromosome = random::instance()->generateDouble(0.0,1.0);
	//Probability to mutate this chromosome
	if (mutate_chromosome <= this->getSettings()->getPropabilityOfMutation()) {
		for (unsigned int i=0; mutant_it != mutant_container.end(); i++){

			double mutate_value = random::instance()->generateDouble(0.0,1.0);
			VALUE_TYPE random_mutation = random::instance()->generateDouble(
					this->getBuilderFactory()->getSettings()->getRandomMax(),
					this->getBuilderFactory()->getSettings()->getRandomMin()
				);

			//Probability to Mutate this value
			if(mutate_value <= this->getSettings()->getAmountOfMutation()) {
				result_container.push_back (random_mutation);
			} else {
				result_container.push_back (*mutant_it);
			}

			//increase iterator
			++mutant_it;

		}
	}

	resultset.push_back(mutation_result);
	return resultset;

}



} //namespace Mutation
} //namespace Operation
} //namespace GeneticLibrary




#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_UNIFORMMUTATIONOPERATION_HPP_ */
