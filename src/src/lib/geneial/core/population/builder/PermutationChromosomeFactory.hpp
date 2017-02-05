#pragma once

#include <geneial/core/population/builder/PermutationChromosomeFactory.h>
#include <geneial/utility/Random.h>

#include <algorithm>
#include <cassert>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{
using ::geneial::population::chromosome::PermutationBuilderSettings;
using ::geneial::population::chromosome::PermutationChromosomeFactory;


geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr PermutationChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>::doCreateChromosome(
        typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues)
{
    using namespace geneial::utility;

    auto new_chromosome = this->allocateNewChromsome();

    if (populateValues == BaseChromosomeFactory<FITNESS_TYPE>::CREATE_VALUES)
    {
    	assert(_settings.getNum() > 1);
        const unsigned int amount = _settings.getNum();

        for(unsigned int i = 0; i < amount; i++)
        {
            const VALUE_TYPE val = i;
            new_chromosome->getContainer()[i] = val;
        }

        const unsigned rounds = Random::generate<unsigned int>(_settings.getPermutationRoundsMin(),_settings.getPermutationRoundsMax());
        for(unsigned int i = 0; i<rounds;i++)
        {
        	unsigned int swapA = Random::generate<unsigned int>(0,_settings.getNum()-1);
        	unsigned int swapB;
        	do{
        		swapB = Random::generate<unsigned int>(0,_settings.getNum()-1);
        	}while(swapA == swapB);
        	auto iterBegin = new_chromosome->getContainer().begin();
        	iter_swap(iterBegin + swapA, iterBegin + swapB);
        }

        assert(new_chromosome->getSize() == _settings.getNum());

    }
    return std::move(new_chromosome);
}

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */
