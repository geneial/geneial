#pragma once

#include <cmath>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>
#include <geneial/utility/Random.h>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>

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

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                typename BaseChromosome<FITNESS_TYPE>::ptr
                ContinousMultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>::doCreateChromosome(
                    typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues)
                {
                    using namespace geneial::utility;

                    const auto minimum = _settings.getRandomMin();
                    const auto maximum = _settings.getRandomMax();
                    const auto epsilon = _settings.getEps();
                    const auto finite = [](VALUE_TYPE value) { return std::isfinite(static_cast<long double>(value)); };
                    if (!finite(minimum) || !finite(maximum) || !finite(epsilon) || minimum > maximum || epsilon < 0 ||
                        (_settings.hasStart() && !finite(_settings.getStartValue())))
                        throw std::invalid_argument(
                            "Continuous factory needs finite ordered limits, a nonnegative step, and a finite anchor");

                    const auto bounds = [=](VALUE_TYPE previous)
                    {
                        VALUE_TYPE lower, upper;
                        if constexpr (std::is_integral_v<VALUE_TYPE>)
                        {
                            using Unsigned = std::make_unsigned_t<
                                std::conditional_t<std::is_same_v<VALUE_TYPE, bool>, unsigned int, VALUE_TYPE>>;
                            constexpr auto lowest = std::numeric_limits<VALUE_TYPE>::lowest();
                            constexpr auto highest = std::numeric_limits<VALUE_TYPE>::max();
                            const auto below =
                                static_cast<Unsigned>(static_cast<Unsigned>(previous) - static_cast<Unsigned>(lowest));
                            const auto above =
                                static_cast<Unsigned>(static_cast<Unsigned>(highest) - static_cast<Unsigned>(previous));
                            lower = below < static_cast<Unsigned>(epsilon)
                                        ? lowest
                                        : static_cast<VALUE_TYPE>(previous - epsilon);
                            upper = above < static_cast<Unsigned>(epsilon)
                                        ? highest
                                        : static_cast<VALUE_TYPE>(previous + epsilon);
                        }
                        else
                        {
                            lower = previous - epsilon;
                            upper = previous + epsilon;
                        }
                        return std::pair{std::max(minimum, lower), std::min(maximum, upper)};
                    };
                    if (_settings.hasStart())
                    {
                        const auto [lower, upper] = bounds(_settings.getStartValue());
                        if (lower > upper)
                            throw std::invalid_argument(
                                "Continuous starting anchor cannot reach the configured interval");
                    }

                    auto new_chromosome = this->allocateNewChromsome();

                    if (populateValues == BaseChromosomeFactory<FITNESS_TYPE>::CREATE_VALUES)
                    {
                        const unsigned int amount = _settings.getNum();

                        unsigned int i = 0;

                        VALUE_TYPE lastVal = 0; // reference to last inserted value

                        while (i < amount)
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

                            const auto [lower_limited, upper_limited] = bounds(lastVal);

                            const VALUE_TYPE val = Random::generate<VALUE_TYPE>(lower_limited, upper_limited);

                            new_chromosome->getContainer()[i] = val;

                            lastVal = val;
                            i++;
                        }
                        assert(new_chromosome->getSize() == _settings.getNum());
                    }

                    return new_chromosome;
                }

            } /* geneial_export_namespace */
        } /* private namespace chromosome */
    } /* private namespace population */
} /* private namespace geneial */
