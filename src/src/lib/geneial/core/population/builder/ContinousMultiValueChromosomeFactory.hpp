#pragma once

#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>
#include <geneial/utility/Random.h>

#include <algorithm>
#include <cassert>

namespace geneial
{
namespace population
{
namespace chromosome
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr ContinousMultiValueChromosomeFactory<VALUE_TYPE,FITNESS_TYPE>::createChromosome(
        typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues)
{
    using namespace geneial::utility;

    typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr new_chromosome(
            new MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>(_settings.getFitnessEvaluator()));
    assert(new_chromosome->getSize() == 0);

    new_chromosome->getContainer().reserve(_settings.getNum());
    if (populateValues == BaseChromosomeFactory<FITNESS_TYPE>::CREATE_VALUES)
    {
        const unsigned int amount = _settings.getNum();

        unsigned int i = amount;

        int lastVal = 0; //reference to last inserted value

        while (i--)
        {

            if (i == amount - 1)
            {
                if (_settings.hasStart())
                {
                    lastVal = _settings.getStart();
                }
                else
                {
                    lastVal = Random::generate<VALUE_TYPE>(_settings.getRandomMin(),
                            _settings.getRandomMax());
                }
            }

            const int val = Random::generate<VALUE_TYPE>(lastVal - _settings.getEps(),
                    lastVal + _settings.getEps());
            const int lower_limited = std::max(_settings.getRandomMin(), val);
            const int upper_limited = std::min(_settings.getRandomMax(), lower_limited);

            new_chromosome->getContainer().push_back(upper_limited);

            lastVal = upper_limited;

        }
        assert(new_chromosome->getSize() == _settings.getNum());

    }

    return new_chromosome;
}

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

