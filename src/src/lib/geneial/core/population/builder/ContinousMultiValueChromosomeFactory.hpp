#pragma once

#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>
#include <geneial/utility/Random.h>

#include <algorithm>
#include <cassert>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{
using ::geneial::population::chromosome::ContinousMultiValueBuilderSettings;
using ::geneial::population::chromosome::ContinousMultiValueChromosomeFactory;


geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr ContinousMultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>::doCreateChromosome(
        typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues)
{
    using namespace geneial::utility;

    auto new_chromosome = this->allocateNewChromsome();

    if (populateValues == BaseChromosomeFactory<FITNESS_TYPE>::CREATE_VALUES)
    {
        const unsigned int amount = _settings.getNum();

        unsigned int i = 0;

        VALUE_TYPE lastVal = 0; //reference to last inserted value

        while (i<amount)
        {

            if (i == 0)
            {
                if (_settings.hasStart())
                {
                    lastVal = _settings.getStartValue();
                }
                else
                {
                    lastVal = Random::generate<VALUE_TYPE>(_settings.getRandomMin(),
                                                           _settings.getRandomMax());
                }
            }

            const VALUE_TYPE lower_limited = std::max(_settings.getRandomMin(), lastVal - _settings.getEps());
            const VALUE_TYPE upper_limited = std::min(_settings.getRandomMax(), lastVal + _settings.getEps());

            const VALUE_TYPE val = Random::generate<VALUE_TYPE>(lower_limited,
                                                                upper_limited);

            new_chromosome->getContainer()[i] = val;

            lastVal = upper_limited;
            i++;
        }
        assert(new_chromosome->getSize() == _settings.getNum());

    }

    return std::move(new_chromosome);
}

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */


