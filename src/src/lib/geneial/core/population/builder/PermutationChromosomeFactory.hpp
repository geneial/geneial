#pragma once

#include <geneial/core/population/builder/PermutationChromosomeFactory.h>
#include <geneial/utility/Random.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <type_traits>

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

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                typename BaseChromosome<FITNESS_TYPE>::ptr
                PermutationChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>::doCreateChromosome(
                    typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues)
                {
                    using namespace geneial::utility;

                    const auto amount = _settings.getNum();
                    if (_settings.getPermutationRoundsMin() > _settings.getPermutationRoundsMax())
                        throw std::invalid_argument("Permutation shuffle-round limits are reversed");
                    const long double largestExact =
                        std::is_integral_v<VALUE_TYPE>
                            ? static_cast<long double>(std::numeric_limits<VALUE_TYPE>::max())
                            : std::ldexp(1.0L, std::numeric_limits<VALUE_TYPE>::digits);
                    if (amount != 0 && static_cast<long double>(amount - 1) > largestExact)
                        throw std::invalid_argument("Permutation gene type cannot represent every index uniquely");

                    auto new_chromosome = this->allocateNewChromsome();

                    if (populateValues == BaseChromosomeFactory<FITNESS_TYPE>::CREATE_VALUES)
                    {

                        for (unsigned int i = 0; i < amount; i++)
                        {
                            const VALUE_TYPE val = i;
                            new_chromosome->getContainer()[i] = val;
                        }

                        if (amount < 2)
                            return new_chromosome;
                        const unsigned rounds = Random::generate<unsigned int>(_settings.getPermutationRoundsMin(),
                                                                               _settings.getPermutationRoundsMax());
                        for (unsigned int i = 0; i < rounds; i++)
                        {
                            unsigned int swapA = Random::generate<unsigned int>(0, _settings.getNum() - 1);
                            unsigned int swapB = Random::generate<unsigned int>(0, amount - 2);
                            if (swapB >= swapA)
                                ++swapB;
                            auto iterBegin = new_chromosome->getContainer().begin();
                            iter_swap(iterBegin + swapA, iterBegin + swapB);
                        }

                        assert(new_chromosome->getSize() == _settings.getNum());
                    }
                    return new_chromosome;
                }

            } /* geneial_export_namespace */
        } /* private namespace chromosome */
    } /* private namespace population */
} /* private namespace geneial */
