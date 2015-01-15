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
#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>
#include <geneial/core/population/Population.h>

using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Operation::Mutation;
using namespace GeneticLibrary::Operation::Choosing;

namespace GeneticLibrary {
namespace Operation {
namespace Mutation {



template<typename VALUE_TYPE, typename FITNESS_TYPE>
/*
 *  Returns a new chromosome which is a partially mutated version of the old one.
 *
 *  */
typename Population::Population<FITNESS_TYPE>::chromosome_container UniformMutationOperation<VALUE_TYPE, FITNESS_TYPE>::doMutate
			(
					typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container _chromosomeInputContainer
			){



	typedef typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::value_container value_container;
	typedef typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr mvc_ptr;
	int pointOfMutation = 0;
	int mutationCounter = 0;
	typename Population::Population<FITNESS_TYPE>::chromosome_container resultset;
	typename Population::Population<FITNESS_TYPE>::chromosome_container _choosenChromosomeContainer;
	typename Population::Population<FITNESS_TYPE>::chromosome_container _notChoosenChromosomeContainer;
	_choosenChromosomeContainer = this->getChoosingOperation()->doChoose(_chromosomeInputContainer);



	//calculates difference: _notChoosenChromosomeContainer = _choosenChromosomeContainer - _chromosomeInputContainer
	std::set_difference(_chromosomeInputContainer.begin(),_chromosomeInputContainer.end(),
				_choosenChromosomeContainer.begin(),_choosenChromosomeContainer.end(),
				std::inserter(_notChoosenChromosomeContainer,_notChoosenChromosomeContainer.begin()));

	//TODO (lukas) compute set difference



	typename Population::Population<FITNESS_TYPE>::chromosome_container::iterator _choosenChromosomeContainer_it;
	for (_choosenChromosomeContainer_it = _choosenChromosomeContainer.begin();
			_choosenChromosomeContainer_it != _choosenChromosomeContainer.end(); ++_choosenChromosomeContainer_it){
			mutationCounter = 0;

			//std::cout << "[";
			//casting mutant as MVC
			mvc_ptr _mvcMutant
					= boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> >(*_choosenChromosomeContainer_it);
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



			//first point of mutation
			if (this->getSettings()->getAmountOfPointsOfMutation()>0){
						 pointOfMutation = random::instance()->generateInt(0,this->getBuilderFactory()->getSettings()->getNum()/this->getSettings()->getAmountOfPointsOfMutation());
						}

			//iterator for one chromosome (to iterate it's values)
			typename value_container::iterator mutant_it = _mutantChromosomeContainer.begin();

			//Insinde Chromosome loop
			for (unsigned int i=0; mutant_it != _mutantChromosomeContainer.end(); i++){

				double value_choise = random::instance()->generateDouble(0.0,1.0);
				//generate a mutation value to replace an old value

				VALUE_TYPE random_mutation = random::instance()->generateDouble(
						this->getBuilderFactory()->getSettings()->getRandomMax(),
						this->getBuilderFactory()->getSettings()->getRandomMin()
					);

				//Check amount of mutation targets in one chromosome (pointsOfMutation)
				if (this->getSettings()->getAmountOfPointsOfMutation()>0){
							//pointOfMutation = Position of Mutation Target
							//Check if current position is a target for mutation.
							if ((i==pointOfMutation) || (this->getSettings()->getAmountOfPointsOfMutation() >= this->getBuilderFactory()->getSettings()->getNum()) ){
								//Check if we reached the maximum points of mutation
								if (this->getSettings()->getAmountOfPointsOfMutation() != mutationCounter){
									//std::cout << mutationCounter+1;
									//add mutation to result_container
									result_container.push_back (random_mutation);
									mutationCounter++;
									//create a new target
									pointOfMutation = random::instance()->generateInt(
											i+1,
											(i+this->getBuilderFactory()->getSettings()->getNum()/this->getSettings()->getAmountOfPointsOfMutation()) % this->getBuilderFactory()->getSettings()->getNum()
										);
								//if no more mutation is needed (mutated already n times)
								} else {
									//std::cout << "-";
									result_container.push_back (*mutant_it);
								}
							//current point is no target
							} else {
								//std::cout << "-";
								result_container.push_back (*mutant_it);
							}

				//Target points are not used for mutation
				} else if(value_choise <= this->getSettings()->getAmountOfMutation()) {
					result_container.push_back (random_mutation);
					//std::cout << "Y";
				} else {
					result_container.push_back (*mutant_it);
					//std::cout << "=";
				}

				//step to next mutant.
				if (mutant_it != _mutantChromosomeContainer.end()) ++mutant_it;
			}

			//Age reset
			_mutatedChromosome->setAge(0);
			resultset.push_back(_mutatedChromosome);

			//std::cout << "]";
	}

	std::cout << "Chrome Loop A" << endl;

	//TODO (lukas) This for loop creates runtime errors!
	//add not mutated Chromosomes
	typename Population::Population<FITNESS_TYPE>::chromosome_container::iterator _notChoosenChromosomeContainer_it;
	for (unsigned int i = 0; _notChoosenChromosomeContainer_it != _notChoosenChromosomeContainer.end(); i++)
	{
		resultset.push_back(*_notChoosenChromosomeContainer_it);
		++_notChoosenChromosomeContainer_it;
	}

	std::cout << "Chrome Loop B" << endl;
	//std::cout << ";" << endl;;
	return resultset;

}

} //namespace Mutation
} //namespace Operation
} //namespace GeneticLibrary




#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_UNIFORMMUTATIONOPERATION_HPP_ */
