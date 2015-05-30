#pragma once

#include <geneial/core/operations/mutation/UniformMutationOperation.h>
#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/population/builder/BuilderSettings.h>
#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>
#include <geneial/core/population/Population.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(mutation)
{
using ::geneial::population::chromosome::BaseChromosomeFactory;
using ::geneial::utility::Random;

geneial_export_namespace
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
        const typename Population<FITNESS_TYPE>::chromosome_container &chromosomeInputContainer,
        BaseManager<FITNESS_TYPE> &manager) const
{

    typename Population<FITNESS_TYPE>::chromosome_container resultset;
    typename Population<FITNESS_TYPE>::chromosome_container choosenChromosomeContainer;
    typename Population<FITNESS_TYPE>::chromosome_container notChoosenChromosomeContainer;

    unsigned int pointOfMutation = 0;
    unsigned int mutationCounter = 0;

    choosenChromosomeContainer = this->getChoosingOperation().doChoose(chromosomeInputContainer);

    //calculates difference: _notChoosenChromosomeContainer = _choosenChromosomeContainer - _chromosomeInputContainer
    std::set_difference(chromosomeInputContainer.begin(), chromosomeInputContainer.end(),
            choosenChromosomeContainer.begin(), choosenChromosomeContainer.end(),
            std::inserter(notChoosenChromosomeContainer, notChoosenChromosomeContainer.begin()));


    const auto randomMin = this->getBuilderFactory().getSettings().getRandomMin();
    const auto randomMax = this->getBuilderFactory().getSettings().getRandomMax() ;
    const auto populationAge = manager.getPopulation().getAge();
    const auto maxNumMvc = this->getBuilderFactory().getSettings().getNum();
    const auto amountPointsOfMutation = this->getSettings().getAmountOfPointsOfMutation();


    //only mutate choosen chromosomes
    for (const auto& chromosome : choosenChromosomeContainer)
    {
        mutationCounter = 0;

        //casting mutant as MVC
        auto mvcMutant = std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(
                chromosome);
        assert(mvcMutant);

        //creating a new MVC (to keep things reversible)
        auto mutatedChromosome = std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(
                this->getBuilderFactory().createChromosome(BaseChromosomeFactory<FITNESS_TYPE>::LET_UNPOPULATED));
        assert(mutatedChromosome);

        //getting values
        const auto &mutantChromosomeContainer = mvcMutant->getContainer();
        auto &result_container = mutatedChromosome->getContainer();

        //first target point of mutation
        if (amountPointsOfMutation > 0)
        {
            pointOfMutation = Random::generate<int>(0, maxNumMvc / amountPointsOfMutation);
        }

        //iterator for one chromosome (to iterate it's values)
        auto mutant_it = mutantChromosomeContainer.cbegin();

        //Inside chromosome loop
        for (unsigned int i = 0; mutant_it != mutantChromosomeContainer.cend(); i++)
        {

            //dicing whether to mutate or not (influeced by propability setting)
            const double value_choice = Random::generate<double>(0.0, 1.0);

            //Check amount of mutation targets in one chromosome (pointsOfMutation)
            if (amountPointsOfMutation > 0)
            {
                //generate a mutation value to replace an old value
                const VALUE_TYPE random_mutation = Random::generate<double>(randomMin, randomMax);

                //pointOfMutation = Position of Mutation Target
                //Check if current position is a target for mutation.
                if ((i == pointOfMutation) || (amountPointsOfMutation >= maxNumMvc))
                {
                    //Check if we reached the maximum points of mutation
                    if (amountPointsOfMutation != mutationCounter)
                    {
                        //add mutation to result_container
                        result_container[i] = (random_mutation);
                        mutationCounter++;
                        //create a new target
                        const unsigned int distanceBetweenTarges = (maxNumMvc / amountPointsOfMutation);
                        pointOfMutation = Random::generate<int>((i + 1), (i + 1 + distanceBetweenTarges));
                        //if no more mutation is needed (mutated already n times)
                    }
                    else
                    {
                        result_container[i] = (*mutant_it);
                    }
                    //current point is no target
                }
                else
                {
                    result_container[i] = (*mutant_it);
                }

                //Target points are not used for mutation
            }
            else if (value_choice <= amountPointsOfMutation)
            {
                //generate a mutation value to replace an old value
                const VALUE_TYPE random_mutation = Random::generate<double>(randomMin, randomMax);

                result_container[i] = (random_mutation);
                //In case dicing (value_choice) choose not to mutate the value
            }
            else
            {
                result_container[i] =(*mutant_it);
            }

            //step to next mutant.
            if (mutant_it != mutantChromosomeContainer.cend())
            {
                ++mutant_it;
            }

        }//inside chromosome loop

        //Age reset
        mutatedChromosome->setAge(0);
        resultset.emplace_back(mutatedChromosome);
    }

    //add not mutated chromosomes
    resultset.insert(resultset.end(), notChoosenChromosomeContainer.begin(), notChoosenChromosomeContainer.end());

    return resultset;

}

} /* geneial_export_namespace */
} /* private namespace mutation */
} /* private namespace operation */
} /* private namespace geneial */

