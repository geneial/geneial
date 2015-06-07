#pragma once

#include <geneial/core/operations/mutation/UniformMutationOperation.h>
#include <geneial/core/population/builder/BuilderSettings.h>
#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>
#include <geneial/core/operations/mutation/MultiValueMutationSettings.h>
#include <geneial/core/population/Population.h>

#include <unordered_set>

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

    choosenChromosomeContainer = this->getChoosingOperation().doChoose(chromosomeInputContainer);

    //calculates difference: _notChoosenChromosomeContainer = _choosenChromosomeContainer - _chromosomeInputContainer
    std::set_difference(chromosomeInputContainer.begin(), chromosomeInputContainer.end(),
            choosenChromosomeContainer.begin(), choosenChromosomeContainer.end(),
            std::inserter(notChoosenChromosomeContainer, notChoosenChromosomeContainer.begin()));


    const auto randomMin = this->getBuilderFactory().getSettings().getRandomMin();
    const auto randomMax = this->getBuilderFactory().getSettings().getRandomMax() ;
    const auto populationAge = manager.getPopulation().getAge();
    const auto maxNumMvc = this->getBuilderFactory().getSettings().getNum();


    //only mutate choosen chromosomes
    for (const auto& chromosome : choosenChromosomeContainer)
    {
        const auto slotsToMutate = Random::generate<unsigned int>(this->getSettings().getMinimumPointsToMutate(),this->getSettings().getMaximumPointsToMutate());
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

        //copy values:
        std::copy(mutantChromosomeContainer.begin(),mutantChromosomeContainer.end(),result_container.begin());

        //Predetermine Positions for Mutation:
        std::unordered_set<unsigned int> positions;
        while (positions.size() < slotsToMutate)
        {
            positions.emplace(Random::generate<unsigned int>(0, maxNumMvc - 1));
        }

        for(const auto pos : positions)
        {
            const VALUE_TYPE randomMutation = Random::generate<VALUE_TYPE>(randomMin, randomMax);
            result_container[pos] = randomMutation;
        }


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

