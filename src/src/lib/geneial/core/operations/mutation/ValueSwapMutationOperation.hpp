#pragma once

#include <geneial/core/operations/mutation/ValueSwapMutationOperation.h>
#include <geneial/core/population/builder/BuilderSettings.h>
#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>
#include <geneial/core/operations/mutation/MultiValueMutationSettings.h>
#include <geneial/core/population/Population.h>

#include <geneial/utility/mixins/Hasher.h>

#include <unordered_set>
#include <iterator>
#include <algorithm>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(mutation)
{
using ::geneial::population::chromosome::BaseChromosomeFactory;
using ::geneial::utility::Random;
using ::geneial::utility::PairHasher;

geneial_export_namespace
{
/*
 *  Returns a chromosome container with some new chromosomes which are partially mutated versions of the old ones.
 *
 *  This mutation swaps elements at random and is intended for permutation chromosomes
 *
 *   Old Chrom.                      New Chrom.
 *      (1)                            (1)
 *      (2) <---\                      (5)
 *      (3)     |                      (3)
 *      (4)     | Swap those Values    (4)
 *      (5) <---/                      (2)
 *      (6)                            (6)
 *      (7)                            (7)
 *
 **/
template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename Population<FITNESS_TYPE>::chromosome_container ValueSwapMutationOperation<VALUE_TYPE, FITNESS_TYPE>::doMutate(
        const typename Population<FITNESS_TYPE>::chromosome_container &chromosomeInputContainer,
        BaseManager<FITNESS_TYPE> &manager) const
{

    typename Population<FITNESS_TYPE>::chromosome_container resultset;
    typename Population<FITNESS_TYPE>::chromosome_container choosenChromosomeContainer;
    typename Population<FITNESS_TYPE>::chromosome_container notChoosenChromosomeContainer;

    choosenChromosomeContainer = this->getChoosingOperation().doChoose(chromosomeInputContainer);

    for(auto it: chromosomeInputContainer)
    {
            auto result = std::find(std::begin(choosenChromosomeContainer), std::end(choosenChromosomeContainer), it);
            if (result == std::end(choosenChromosomeContainer))
            {
                 notChoosenChromosomeContainer.emplace_back(it);
            }
    }



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
        std::unordered_set<std::pair<unsigned int,unsigned int>,PairHasher> positionsToSwap;
        while (positionsToSwap.size() < slotsToMutate)
        {
        	unsigned int swapA = Random::generate<unsigned int>(0,maxNumMvc - 1);
			unsigned int swapB;
			do{
				swapB = Random::generate<unsigned int>(0, maxNumMvc - 1);
			}while(swapA == swapB);
        	positionsToSwap.emplace(std::make_pair(swapA,swapB));
        }

        for(const auto posPair : positionsToSwap)
        {
        	auto iterBegin = result_container.begin();
        	iter_swap(iterBegin + posPair.first, iterBegin + posPair.second);
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

