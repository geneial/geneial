#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/builder/BaseChromosomeFactory.h>
#include <geneial/core/population/builder/MultiValueBuilderSettings.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <geneial/utility/Random.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{
using ::geneial::utility::Random;

geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>::doCreateChromosome(
        typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues)
{

    auto new_chromosome = this->allocateNewChromsome();


    if (populateValues == BaseChromosomeFactory<FITNESS_TYPE>::CREATE_VALUES)
    {
        auto &container= new_chromosome->getContainer();
        for(unsigned int i=0;i<this->_settings.getNum();i++)
        {
            container[i] = Random::generate<VALUE_TYPE>(this->_settings.getRandomMin(), this->_settings.getRandomMax());
        }
    }

    assert(new_chromosome->getSize() == _settings.getNum());

    return std::move(new_chromosome);
}

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */
