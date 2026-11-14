#pragma once

#include <geneial/core/operations/selection/RouletteWheelSelection.h>
#include <geneial/utility/Random.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <vector>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(operation)
    {
        geneial_private_namespace(selection)
        {
            using ::geneial::population::Population;
            using ::geneial::population::management::BaseManager;
            using ::geneial::utility::Random;

            geneial_export_namespace
            {

                template <typename FITNESS_TYPE> class RouletteWheel
                {
                  private:
                    using chrom_ptr_type = typename BaseChromosome<FITNESS_TYPE>::ptr;
                    std::vector<std::pair<long double, chrom_ptr_type>> _weights;

                  public:
                    explicit RouletteWheel(const Population<FITNESS_TYPE> &population)
                    {
                        if (population.getSize() == 0)
                        {
                            throw std::invalid_argument("Cannot select from an empty population");
                        }
                        const long double worst = population.getFitnessMap().begin()->first;
                        const long double offset = std::min(0.0L, worst);
                        long double scale = 1;
                        for (const auto &entry : population.getFitnessMap())
                        {
                            if (!std::isfinite(static_cast<long double>(entry.first)))
                                throw std::invalid_argument("Roulette fitness must be finite");
                            scale = std::max(scale, std::abs(static_cast<long double>(entry.first)));
                        }
                        for (const auto &entry : population.getFitnessMap())
                        {
                            const long double weight =
                                static_cast<long double>(entry.first) / scale - offset / scale + 1.0L / scale;
                            if (!std::isfinite(weight) || weight <= 0)
                            {
                                throw std::invalid_argument("Roulette fitness must produce finite positive weights");
                            }
                            _weights.emplace_back(weight, entry.second);
                        }
                    }

                    chrom_ptr_type spin(double random) const
                    {
                        if (_weights.empty() || !(random >= 0 && random <= 1))
                        {
                            throw std::invalid_argument("Cannot spin an empty wheel or use a value outside [0, 1]");
                        }
                        long double sum = 0;
                        for (const auto &entry : _weights)
                        {
                            sum += entry.first;
                        }
                        long double position = random * sum;
                        for (const auto &entry : _weights)
                        {
                            if (position < entry.first)
                            {
                                return entry.second;
                            }
                            position -= entry.first;
                        }
                        return _weights.back().second;
                    }

                    void remove(const chrom_ptr_type &chromosome)
                    {
                        _weights.erase(std::remove_if(_weights.begin(), _weights.end(),
                                                      [&chromosome](const std::pair<long double, chrom_ptr_type> &entry)
                                                      { return entry.second == chromosome; }),
                                       _weights.end());
                    }
                };

                template <typename FITNESS_TYPE>
                typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set
                RouletteWheelSelection<FITNESS_TYPE>::doSelect(const Population<FITNESS_TYPE> &population,
                                                               BaseManager<FITNESS_TYPE> &manager) const
                {
                    const auto count = this->getSettings().getNumberOfParents();
                    if (count > population.getSize())
                    {
                        throw std::invalid_argument("Parent count exceeds population size");
                    }
                    typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set result;
                    if (count == 0)
                    {
                        return result;
                    }
                    RouletteWheel<FITNESS_TYPE> wheel(population);
                    result.reserve(count);
                    for (unsigned int index = 0; index < count; ++index)
                    {
                        auto chromosome = wheel.spin(Random::generate<double>());
                        result.emplace_back(chromosome);
                        wheel.remove(chromosome);
                    }
                    return result;
                }
            }
        }
    }
}
