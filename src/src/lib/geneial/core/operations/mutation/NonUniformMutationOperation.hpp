/*
 * NonUniformMutationOperation.hpp
 *
 *  Created on: Dec 30, 2014
 *      Author: lukas
 */

#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_NONUNIFORMMUTATIONOPERATION_HPP_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_NONUNIFORMMUTATIONOPERATION_HPP_

#include <geneial/core/population/Population.h>
#include <geneial/core/operations/mutation/NonUniformMutationOperation.h>
#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/population/builder/BuilderSettings.h>
#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>


namespace GeneticLibrary {
namespace Operation {
namespace Mutation {

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Operation::Mutation;
using namespace GeneticLibrary::Operation::Choosing;

/*
 *  Returns a chromosome container with some new chromosomes which are partially mutated versions of the old ones.
 *
 *  Targetpoints for mutation represent the chosen values within an Chromosome to be changed
 *  Example for 3 Points of Mutation:
 *
 *   Old Chrom.					  New Chrom.
 *  	(X) <- Mutate this value --> (Y)
 *	 	(X)							 (X)
 *  	(X)						     (X)
 *  	(X) <- Mutate this value --> (Y)
 *  	(X)							 (X)
 *  	(X) <- Mutate this value --> (Y)
 *  	(X)							 (X)
 *
 *  */
template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename Population<FITNESS_TYPE>::chromosome_container NonUniformMutationOperation<VALUE_TYPE, FITNESS_TYPE>::doMutate
			(
					typename Population<FITNESS_TYPE>::chromosome_container _chromosomeInputContainer,
					BaseManager<FITNESS_TYPE> &manager
			)
{

	typedef typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::value_container value_container;
	typedef typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr mvc_ptr;

	typename Population<FITNESS_TYPE>::chromosome_container resultset;
	typename Population<FITNESS_TYPE>::chromosome_container choosenChromosomeContainer;
	typename Population<FITNESS_TYPE>::chromosome_container notChoosenChromosomeContainer;

	unsigned int pointOfMutation = 0;
	unsigned int mutationCounter = 0;
	double split = 0;
	double value_choice = 0;

	choosenChromosomeContainer = this->getChoosingOperation()->doChoose(_chromosomeInputContainer);

	//calculates difference: _notChoosenChromosomeContainer = _choosenChromosomeContainer - _chromosomeInputContainer
	std::set_difference(_chromosomeInputContainer.begin(),_chromosomeInputContainer.end(),
				choosenChromosomeContainer.begin(),choosenChromosomeContainer.end(),
				std::inserter(notChoosenChromosomeContainer,notChoosenChromosomeContainer.begin()));

	typename Population<FITNESS_TYPE>::chromosome_container::iterator _choosenChromosomeContainer_it;

	//only mutate choosen chromosomes
	for (_choosenChromosomeContainer_it = choosenChromosomeContainer.begin();
			_choosenChromosomeContainer_it != choosenChromosomeContainer.end(); ++_choosenChromosomeContainer_it)
	{
			mutationCounter = 0;

			//casting mutant as MVC
			mvc_ptr _mvcMutant
					= boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> >(*_choosenChromosomeContainer_it);
			assert(_mvcMutant);


			//TODO(bewo): avoid creating a fully featured chromosome for perf, rather use some kind of prototype here?!
			//creating a new MVC (to keep things reversible)
			mvc_ptr _mutatedChromosome =
						boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> >(
								this->getBuilderFactory()->createChromosome(false)
						);
			assert(_mutatedChromosome);

			value_container &_mutantChromosomeContainer = _mvcMutant->getContainer();
			value_container &result_container = _mutatedChromosome->getContainer();
			result_container.clear();

			//first target point of mutation
			if (this->getSettings()->getAmountOfPointsOfMutation()>0)
			{
				pointOfMutation = Random::instance()->generateInt(0,this->getBuilderFactory()->getSettings()->getNum()/this->getSettings()->getAmountOfPointsOfMutation());
			}

			//iterator for one chromosome (to iterate it's values)
			typename value_container::iterator mutant_it = _mutantChromosomeContainer.begin();

			//Insinde Chromosome loop
			for (unsigned int i=0; mutant_it != _mutantChromosomeContainer.end(); i++)
			{


				//dicing whether to mutate or not (influeced by propability setting)
				value_choice = Random::instance()->generateDouble(0.0,1.0);

				/*
				 * Creates a split which shifts the weight from the random value to the old value.
				 * The replaced value is a sum from random and old value.
				 */
				if (manager.getPopulation().getAge() > 0)
				{
					split = 1-static_cast<double>(manager.getPopulation().getAge()) / static_cast<double>(this->getAffectedGenerations());
				}


				//keeping a minimum amount of mutation
				if (this->getAffectedGenerations() < manager.getPopulation().getAge())
				{
					split = this->getMinimumModification();
				}

				if (split < this->getMinimumModification())
				{
					split = this->getMinimumModification();
				}

				VALUE_TYPE randomMutation = Random::instance()->generateDouble(
						this->getBuilderFactory()->getSettings()->getRandomMin(),
						this->getBuilderFactory()->getSettings()->getRandomMax()
					);
				double weightedMutation = ((split)*randomMutation + (1-split)* *mutant_it);


				//Check amount of mutation targets in one chromosome (pointsOfMutation)
				if (this->getSettings()->getAmountOfPointsOfMutation() > 0)
				{
					//pointOfMutation = Position of Mutation Target
					//Check if current position is a target for mutation.
					if ((i==pointOfMutation) || (this->getSettings()->getAmountOfPointsOfMutation() >= this->getBuilderFactory()->getSettings()->getNum()) )
					{
						//Check if we reached the maximum points of mutation
						if (this->getSettings()->getAmountOfPointsOfMutation() != mutationCounter)
						{

							//add mutation to result_container
							result_container.push_back (int (weightedMutation));
							mutationCounter++;

							//create a new target
							const unsigned int distanceBetweenTarges = (this->getBuilderFactory()->getSettings()->getNum() / this->getSettings()->getAmountOfPointsOfMutation());
							pointOfMutation = Random::instance()->generateInt(
									(i+1),
									(i+1+distanceBetweenTarges)
							);
						//if no more mutation is needed (mutated already n times)
						} else {
							result_container.push_back (*mutant_it);
						}
					//current point is no target
					} else {
						result_container.push_back (*mutant_it);
					}


				//Target points are not used for mutation
				}
				else if(value_choice <= this->getSettings()->getAmountOfMutation())
				{
					result_container.push_back (int (weightedMutation));
				//In case dicing (value_choice) choose not to mutate the value
				} else {
					result_container.push_back (*mutant_it);
				}

				//step to next mutant.
				if (mutant_it != _mutantChromosomeContainer.end())
				{
					++mutant_it;
				}

			}//inside chromosome loop

			//Age reset
			_mutatedChromosome->setAge(0);
			resultset.push_back(_mutatedChromosome);
	}


	//add not mutated Chromosomes
	resultset.insert(resultset.end(),notChoosenChromosomeContainer.begin(),notChoosenChromosomeContainer.end());

	return resultset;

}



} //namespace Mutation
} //namespace Operation
} //namespace GeneticLibrary





#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_NONUNIFORMMUTATIONOPERATION_HPP_ */
