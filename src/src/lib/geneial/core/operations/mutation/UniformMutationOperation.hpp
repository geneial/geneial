#pragma once

#include <geneial/core/operations/mutation/UniformMutationOperation.h>
#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/population/builder/BuilderSettings.h>
#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>
#include <geneial/core/population/Population.h>

namespace geneial
{
namespace operation
{
namespace mutation
{
// TODO(bewo): reduce cc!
/*
 *  Returns a chromosome container with some new chromosomes which are partially mutated versions of the old ones.
 *
 *  Targetpoints for mutation represent the choosen values within an Chromosome to be changed
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
 *
 **/
template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename Population<FITNESS_TYPE>::chromosome_container UniformMutationOperation<VALUE_TYPE, FITNESS_TYPE>::doMutate(
        const typename Population<FITNESS_TYPE>::chromosome_container &_chromosomeInputContainer,
        BaseManager<FITNESS_TYPE> &manager)
{

    typedef typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::value_container value_container;
    typedef typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr mvc_ptr;

    typename Population<FITNESS_TYPE>::chromosome_container resultset;
    typename Population<FITNESS_TYPE>::chromosome_container choosenChromosomeContainer;
    typename Population<FITNESS_TYPE>::chromosome_container notChoosenChromosomeContainer;

    unsigned int pointOfMutation = 0;
    unsigned int mutationCounter = 0;

    choosenChromosomeContainer = this->getChoosingOperation().doChoose(_chromosomeInputContainer);

    //calculates difference: _notChoosenChromosomeContainer = _choosenChromosomeContainer - _chromosomeInputContainer
    std::set_difference(_chromosomeInputContainer.begin(), _chromosomeInputContainer.end(),
            choosenChromosomeContainer.begin(), choosenChromosomeContainer.end(),
            std::inserter(notChoosenChromosomeContainer, notChoosenChromosomeContainer.begin()));

    typename Population<FITNESS_TYPE>::chromosome_container::iterator choosenChromosomeContainer_it;

    //only mutate choosen chromosomes
    for (choosenChromosomeContainer_it = choosenChromosomeContainer.begin();
            choosenChromosomeContainer_it != choosenChromosomeContainer.end(); ++choosenChromosomeContainer_it)
    {
        mutationCounter = 0;

        //casting mutant as MVC
        mvc_ptr mvcMutant = boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(
                *choosenChromosomeContainer_it);
        assert(mvcMutant);

        //creating a new MVC (to keep things reversible)
        mvc_ptr mutatedChromosome = boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(
                this->getBuilderFactory().createChromosome(BaseChromosomeFactory<FITNESS_TYPE>::LET_UNPOPULATED));
        assert(mutatedChromosome);

        //getting values
        value_container &mutantChromosomeContainer = mvcMutant->getContainer();
        value_container &result_container = mutatedChromosome->getContainer();
        result_container.clear();

        //first target point of mutation
        if (this->_settings.getAmountOfPointsOfMutation() > 0)
        {
            pointOfMutation = Random::generate<int>(0,
                    this->_builderFactory.getSettings().getNum()
                            / this->_settings.getAmountOfPointsOfMutation());
        }

        //iterator for one chromosome (to iterate it's values)
        typename value_container::iterator mutant_it = mutantChromosomeContainer.begin();

        //Inside chromosome loop
        for (unsigned int i = 0; mutant_it != mutantChromosomeContainer.end(); i++)
        {

            //dicing whether to mutate or not (influeced by propability setting)
            double value_choice = Random::generate<double>(0.0, 1.0);

            //generate a mutation value to replace an old value
            VALUE_TYPE random_mutation = Random::generate<double>(
                    this->_builderFactory.getSettings().getRandomMin(),
                    this->_builderFactory.getSettings().getRandomMax());

            //Check amount of mutation targets in one chromosome (pointsOfMutation)
            if (this->getSettings().getAmountOfPointsOfMutation() > 0)
            {
                //pointOfMutation = Position of Mutation Target
                //Check if current position is a target for mutation.
                if ((i == pointOfMutation)
                        || (this->_settings.getAmountOfPointsOfMutation()
                                >= this->_builderFactory.getSettings().getNum()))
                {
                    //Check if we reached the maximum points of mutation
                    if (this->_settings.getAmountOfPointsOfMutation() != mutationCounter)
                    {
                        //add mutation to result_container
                        result_container.push_back(random_mutation);
                        mutationCounter++;
                        //create a new target
                        const unsigned int distanceBetweenTarges = (this->_builderFactory.getSettings().getNum()
                                / this->_settings.getAmountOfPointsOfMutation());
                        pointOfMutation = Random::generate<int>((i + 1), (i + 1 + distanceBetweenTarges));
                        //if no more mutation is needed (mutated already n times)
                    }
                    else
                    {
                        result_container.push_back(*mutant_it);
                    }
                    //current point is no target
                }
                else
                {
                    result_container.push_back(*mutant_it);
                }

                //Target points are not used for mutation
            }
            else if (value_choice <= this->_settings.getAmountOfMutation())
            {
                result_container.push_back(random_mutation);
                //In case dicing (value_choice) choose not to mutate the value
            }
            else
            {
                result_container.push_back(*mutant_it);
            }

            //step to next mutant.
            if (mutant_it != mutantChromosomeContainer.end())
            {
                ++mutant_it;
            }

        }//inside chromosome loop

        //Age reset
        mutatedChromosome->setAge(0);
        resultset.push_back(mutatedChromosome);
    }

    //add not mutated chromosomes
    resultset.insert(resultset.end(), notChoosenChromosomeContainer.begin(), notChoosenChromosomeContainer.end());

    return resultset;

}

} /* namespace mutation */
} /* namespace operation */
} /* namespace geneial */

