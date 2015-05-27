#pragma once

#include <geneial/core/population/Population.h>
#include <geneial/core/operations/mutation/NonUniformMutationOperation.h>
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
using ::geneial::population::Population;

geneial_export_namespace
{
/*
 *  Returns a chromosome container with some new chromosomes which are partially mutated versions of the old ones.
 *
 *  target points for mutation represent the chosen values within an chromosome to be changed
 *  Example for 3 points of Mutation:
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
 * TODO (bewo): reduce cyclomatic complexity!
 *
 *  */
template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename Population<FITNESS_TYPE>::chromosome_container NonUniformMutationOperation<VALUE_TYPE, FITNESS_TYPE>::doMutate(
        const typename Population<FITNESS_TYPE>::chromosome_container &chromosomeInputContainer,
        BaseManager<FITNESS_TYPE> &manager) const
{

    typename Population<FITNESS_TYPE>::chromosome_container notChoosenChromosomeContainer;


    const auto randomMin = this->getBuilderFactory().getSettings().getRandomMin();
    const auto randomMax = this->getBuilderFactory().getSettings().getRandomMax() ;
    const auto populationAge = manager.getPopulation().getAge();
    const auto maxNumMvc = this->getBuilderFactory().getSettings().getNum();
    const auto affectedGenerations = this->getAffectedGenerations();
    const auto amountPointsOfMutation = this->getSettings().getAmountOfPointsOfMutation();


    unsigned int pointOfMutation = 0;
    unsigned int mutationCounter = 0;
    double split = 0;
    double value_choice = 0;


    auto choosenChromosomeContainer(this->getChoosingOperation().doChoose(chromosomeInputContainer));

    //calculates difference: notChoosenChromosomeContainer = _choosenChromosomeContainer - _chromosomeInputContainer

    std::set_difference(chromosomeInputContainer.cbegin(), chromosomeInputContainer.cend(),
            choosenChromosomeContainer.cbegin(), choosenChromosomeContainer.cend(),
            std::back_inserter(notChoosenChromosomeContainer)); //TODO(bewo): This seems unnecessarily expensive..

    typename Population<FITNESS_TYPE>::chromosome_container resultset;

    //choosen container is an upper bound..
    resultset.reserve(choosenChromosomeContainer.size());


    //only mutate choosen chromosomes
    for (const auto& chosenChromosome : choosenChromosomeContainer)
    {
        mutationCounter = 0;
        //casting mutant as MVC
        const auto mvcMutant(std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(
                chosenChromosome));

        assert(mvcMutant);

        auto mutatedChromosome = std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(
                this->getBuilderFactory().createChromosome(BaseChromosomeFactory<FITNESS_TYPE>::LET_UNPOPULATED));
        assert(mutatedChromosome);

        const auto &_mutantChromosomeContainer = mvcMutant->getContainer();
        auto &result_container = mutatedChromosome->getContainer();

        //first target point of mutation
        if (amountPointsOfMutation > 0)
        {
            pointOfMutation = Random::generate<int>(0, maxNumMvc / amountPointsOfMutation);
        }

        //iterator for one chromosome (to iterate it's values)
        auto mutant_it = _mutantChromosomeContainer.cbegin();

        //Insinde Chromosome loop
        for (unsigned int i = 0; mutant_it != _mutantChromosomeContainer.cend(); i++)
        {

            //dicing whether to mutate or not (influeced by propability setting)
            value_choice = Random::generate<double>(0.0, 1.0);

            /*
             * Creates a split which shifts the weight from the random value to the old value.
             * The replaced value is a sum from random and old value.
             */
            if (populationAge > 0)
            {
                split = 1 - static_cast<double>(populationAge) / static_cast<double>(affectedGenerations);
            }

            //keeping a minimum amount of mutation
            if (affectedGenerations < populationAge)
            {
                split = this->getMinimumModification();
            }

            if (split < this->getMinimumModification())
            {
                split = this->getMinimumModification();
            }

            const VALUE_TYPE randomMutation = Random::generate<VALUE_TYPE>(randomMin, randomMax);
            const VALUE_TYPE weightedMutation = ((split) * randomMutation + (1 - split) * *mutant_it);

            //Check amount of mutation targets in one chromosome (pointsOfMutation)
            if (this->getSettings().getAmountOfPointsOfMutation() > 0)
            {
                //pointOfMutation = Position of Mutation Target
                //Check if current position is a target for mutation.
                if ((i == pointOfMutation) || (amountPointsOfMutation >= maxNumMvc))
                {
                    //Check if we reached the maximum points of mutation
                    if (amountPointsOfMutation != mutationCounter)
                    {
                        //add mutation to result_container
                        result_container[i] = weightedMutation;
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
                result_container[i] = (weightedMutation);
                //In case dicing (value_choice) choose not to mutate the value
            }
            else
            {
                result_container[i] = (*mutant_it);
            }

            //step to next mutant.
            if (mutant_it != _mutantChromosomeContainer.cend())
            {
                ++mutant_it;
            }

        }

        mutatedChromosome->setAge(0);
        resultset.push_back(mutatedChromosome);
    }

    //add not mutated chromosomes
    resultset.insert(resultset.end(), notChoosenChromosomeContainer.begin(), notChoosenChromosomeContainer.end());

    return resultset;

}

} /* geneial_export_namespace */
} /* private namespace mutation */
} /* private namespace operation */
} /* private namespace geneial */

