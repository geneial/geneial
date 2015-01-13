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
typename BaseMutationOperation<FITNESS_TYPE>::mutation_result_set UniformMutationOperation<VALUE_TYPE, FITNESS_TYPE>::doMutate
			(
					typename Mutation::BaseMutationOperation<FITNESS_TYPE>::mutation_result_set _chromosomeInputContainer
			){

	typedef typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::value_container value_container;
	typedef typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr mvc_ptr;
	int _pointOfMutation;
	int _mutationCounter;
	typename Mutation::BaseMutationOperation<FITNESS_TYPE>::mutation_result_set resultset;

	typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container::iterator _chromosomeInputContainer_it;
	for (_chromosomeInputContainer_it = _chromosomeInputContainer.begin();
			_chromosomeInputContainer_it != _chromosomeInputContainer.end(); ++_chromosomeInputContainer_it){
			_mutationCounter = 0;
			//std::cout << "[";
			//casting mutant as MVC
			mvc_ptr _mvcMutant
					= boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> >(*_chromosomeInputContainer_it);
			assert(_mvcMutant);

			//creating a new MVC (to keep things reversible)
			mvc_ptr _mutatedChromosome =
						boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> >(
								this->getBuilderFactory()->createChromosome()
						);
			assert(_mutatedChromosome);


			//getting values
			value_container &_mutantChromosomeContainer = _mvcMutant->getContainer();
			value_container &result_container = _mutatedChromosome->getContainer();
			result_container.clear();

			//iterator for one chromosome (to iterate it's values)
			typename value_container::iterator mutant_it = _mutantChromosomeContainer.begin();

			//TODO (Lukas) computation cost?
			double chromosome_choise = random::instance()->generateDouble(0.0,1.0);

			//Probability to mutate this chromosome
			if (chromosome_choise <= this->getSettings()->getPropabilityOfMutation()) {

				//first point of mutation
				if (this->getSettings()->getAmountOfPointsOfMutation()>0){
							 _pointOfMutation = random::instance()->generateInt(0,this->getBuilderFactory()->getSettings()->getNum()/this->getSettings()->getAmountOfPointsOfMutation());
							}

				for (unsigned int i=0; mutant_it != _mutantChromosomeContainer.end(); i++){

					double value_choise = random::instance()->generateDouble(0.0,1.0);
					VALUE_TYPE random_mutation = random::instance()->generateDouble(
							this->getBuilderFactory()->getSettings()->getRandomMax(),
							this->getBuilderFactory()->getSettings()->getRandomMin()
						);

					//next point of mutation
					if (this->getSettings()->getAmountOfPointsOfMutation()>0){
								if ((i==_pointOfMutation) || (this->getSettings()->getAmountOfPointsOfMutation() >= this->getBuilderFactory()->getSettings()->getNum()) ){
									if (this->getSettings()->getAmountOfPointsOfMutation() != _mutationCounter){
										//std::cout << _mutationCounter+1;
										result_container.push_back (random_mutation);
										_mutationCounter++;
										_pointOfMutation = random::instance()->generateInt(
												i+1,
												(i+this->getBuilderFactory()->getSettings()->getNum()/this->getSettings()->getAmountOfPointsOfMutation()) % this->getBuilderFactory()->getSettings()->getNum()
											);
									} else {
										//std::cout << "-";
										result_container.push_back (*mutant_it);
									}
								} else {
									//std::cout << "-";
									result_container.push_back (*mutant_it);
								}


					} else if(value_choise <= this->getSettings()->getAmountOfMutation()) {
						result_container.push_back (random_mutation);
						//std::cout << "Y";
					} else {
						result_container.push_back (*mutant_it);
						//std::cout << "=";
					}

					//increase iterator
					++mutant_it;

				}

				//Age reset
				_mutatedChromosome->setAge(0);

				resultset.push_back(_mutatedChromosome);
			} else {
				//std::cout << "+";
				resultset.push_back(*_chromosomeInputContainer_it);
			}
			//std::cout << "]";
	}

	//std::cout << ";" << endl;;
	return resultset;

}

} //namespace Mutation
} //namespace Operation
} //namespace GeneticLibrary




#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_UNIFORMMUTATIONOPERATION_HPP_ */
