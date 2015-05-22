#pragma once

#include <geneial/core/population/builder/BaseChromosomeFactory.h>
#include <geneial/core/population/builder/MultiValueBuilderSettings.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <geneial/utility/Random.h>

namespace geneial
{
namespace population
{
namespace chromosome
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>::createChromosome(
        typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues) const
{

    typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr new_chromosome(
            new MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>(this->_settings.getFitnessEvaluator()));
    assert(new_chromosome->getSize() == 0);

    new_chromosome->getContainer().reserve(this->_settings.getNum());

    if (populateValues == BaseChromosomeFactory<FITNESS_TYPE>::CREATE_VALUES)
    {
        int i = this->_settings.getNum();
        while (i--)
        {
            new_chromosome->getContainer().push_back(
                    Random::generate<VALUE_TYPE>(this->_settings.getRandomMin(), this->_settings.getRandomMax()));
        }
        assert(new_chromosome->getSize() == _settings.getNum());

    }

    return std::move(new_chromosome);
}

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */
