#pragma once

#include <geneial/namespaces.h>

#include <geneial/core/population/chromosome/MultiValueChromosome.h>

#include <functional>
#include <numeric>

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <type_traits>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(population)
    {
        geneial_private_namespace(chromosome)
        {

            geneial_export_namespace
            {

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                bool MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::equals(
                    const BaseChromosome<FITNESS_TYPE> &chromosome) const
                {
                    const auto *other = dynamic_cast<const MultiValueChromosome *>(&chromosome);
                    return other && _container == other->_container;
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                typename BaseChromosome<FITNESS_TYPE>::chromsome_hash
                MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getHash() const
                {
#ifndef NMVCHASH_CACHE
                    if (!hasCache())
                    {
#endif
                        // TODO (bewo): Introduce a mutex lock here for threading.
                        _hashCache = _container.size();
                        for (const auto &value : _container)
                        {
                            _hashCache ^=
                                std::hash<VALUE_TYPE>{}(value) + 0x9e3779b9 + (_hashCache << 6) + (_hashCache >> 2);
                        }
#ifndef NMVCHASH_CACHE
                        _cacheValid = true;
                    }
#endif
                    typename BaseChromosome<FITNESS_TYPE>::chromsome_hash result = _hashCache;
                    return result;
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                unsigned int MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getSize() const
                {
                    return _container.size();
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::value_container &
                MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getContainer()
                {
                    invalidateHashCache();
                    this->invalidateFitness();
                    return _container;
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::value_container &
                MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getContainer() const
                {
                    return _container;
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                bool inline MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::hasCache() const
                {
                    return _cacheValid;
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                void inline MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::invalidateHashCache() const
                {
                    _cacheValid = false;
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                void MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::setValueContainer(const value_container &container)
                {
                    invalidateHashCache();
                    this->invalidateFitness();
                    _container = container;
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                void MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::setValueContainer(value_container && container)
                {
                    invalidateHashCache();
                    this->invalidateFitness();
                    std::swap(_container, container);
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_it
                MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getConstIt() const
                {
                    return _container.begin();
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::it
                MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getIt()
                {
                    invalidateHashCache();
                    this->invalidateFitness();
                    return _container.begin();
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_it
                MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getMax() const
                {
                    return std::max_element(_container.begin(), _container.end());
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_it
                MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getMin() const
                {
                    return std::min_element(_container.begin(), _container.end());
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                VALUE_TYPE MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getAverage() const
                {
                    if (_container.empty())
                    {
                        return VALUE_TYPE{};
                    }
                    if constexpr (std::is_integral_v<VALUE_TYPE>)
                    {
                        struct DividedSum
                        {
                            std::uintmax_t quotient = 0;
                            std::uintmax_t remainder = 0;

                            void add(std::uintmax_t magnitude, std::uintmax_t divisor)
                            {
                                quotient += magnitude / divisor;
                                const auto residual = magnitude % divisor;
                                if (remainder >= divisor - residual)
                                {
                                    ++quotient;
                                    remainder -= divisor - residual;
                                }
                                else
                                {
                                    remainder += residual;
                                }
                            }
                        };
                        DividedSum positive, negative;
                        const auto divisor = static_cast<std::uintmax_t>(_container.size());
                        for (const auto value : _container)
                        {
                            if constexpr (std::is_signed_v<VALUE_TYPE>)
                            {
                                if (value < 0)
                                {
                                    negative.add(std::uintmax_t{} - static_cast<std::uintmax_t>(value), divisor);
                                    continue;
                                }
                            }
                            positive.add(static_cast<std::uintmax_t>(value), divisor);
                        }
                        const bool isNegative = negative.quotient > positive.quotient;
                        const auto &larger = isNegative ? negative : positive;
                        const auto &smaller = isNegative ? positive : negative;
                        auto magnitude = larger.quotient - smaller.quotient;
                        if (magnitude != 0 && larger.remainder < smaller.remainder)
                        {
                            --magnitude;
                        }
                        if constexpr (std::is_signed_v<VALUE_TYPE>)
                        {
                            if (isNegative && magnitude != 0)
                            {
                                return static_cast<VALUE_TYPE>(-static_cast<VALUE_TYPE>(magnitude - 1) - 1);
                            }
                        }
                        return static_cast<VALUE_TYPE>(magnitude);
                    }
                    else
                    {
                        const auto sum = std::accumulate(_container.begin(), _container.end(), VALUE_TYPE{});
                        return static_cast<VALUE_TYPE>(static_cast<long double>(sum) / _container.size());
                    }
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                VALUE_TYPE MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getRange() const
                {
                    if (_container.empty())
                    {
                        return VALUE_TYPE{};
                    }
                    const auto [minimum, maximum] = std::minmax_element(_container.begin(), _container.end());
                    if constexpr (std::is_integral_v<VALUE_TYPE> && std::is_signed_v<VALUE_TYPE>)
                    {
                        if (*minimum < 0 && *maximum > std::numeric_limits<VALUE_TYPE>::max() + *minimum)
                        {
                            throw std::overflow_error("Chromosome range cannot be represented by its gene type");
                        }
                    }
                    return static_cast<VALUE_TYPE>(*maximum - *minimum);
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                VALUE_TYPE MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getSum() const
                {
                    if constexpr (std::is_integral_v<VALUE_TYPE>)
                    {
                        VALUE_TYPE sum{};
                        if constexpr (std::is_signed_v<VALUE_TYPE>)
                        {
                            const auto isPositive = [](VALUE_TYPE value) { return value > 0; };
                            const auto isNegative = [](VALUE_TYPE value) { return value < 0; };
                            auto positive = std::find_if(_container.begin(), _container.end(), isPositive);
                            auto negative = std::find_if(_container.begin(), _container.end(), isNegative);
                            while (positive != _container.end() || negative != _container.end())
                            {
                                VALUE_TYPE value;
                                if ((sum <= 0 && positive != _container.end()) || negative == _container.end())
                                {
                                    value = *positive;
                                    positive = std::find_if(std::next(positive), _container.end(), isPositive);
                                }
                                else
                                {
                                    value = *negative;
                                    negative = std::find_if(std::next(negative), _container.end(), isNegative);
                                }
                                if ((value > 0 && sum > std::numeric_limits<VALUE_TYPE>::max() - value) ||
                                    (value < 0 && sum < std::numeric_limits<VALUE_TYPE>::lowest() - value))
                                {
                                    throw std::overflow_error("Chromosome sum cannot be represented by its gene type");
                                }
                                sum = static_cast<VALUE_TYPE>(sum + value);
                            }
                        }
                        else
                        {
                            for (const auto value : _container)
                            {
                                if (value > std::numeric_limits<VALUE_TYPE>::max() - sum)
                                {
                                    throw std::overflow_error("Chromosome sum cannot be represented by its gene type");
                                }
                                sum = static_cast<VALUE_TYPE>(sum + value);
                            }
                        }
                        return sum;
                    }
                    else
                    {
                        return std::accumulate(_container.begin(), _container.end(), VALUE_TYPE{});
                    }
                }

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                void MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::print(std::ostream & os) const
                {
                    os << "Fitness: ";
                    if (this->hasFitness())
                    {
                        os << this->getFitness();
                    }
                    else
                    {
                        os << "N/A";
                    }
                    os << ", ";
                    os << "Age: " << this->getAge() << ", ";

                    os << "Hash: ";
                    this->printHash(os);
                    os << std::endl;

                    os << "Values: " << std::endl;
                    std::ostream_iterator<VALUE_TYPE> out_it(os, "; ");
                    std::copy(_container.begin(), _container.end(), out_it);
                    os << std::endl;
                }

            } /* geneial_export_namespace */
        } /* private namespace chromosome */
    } /* private namespace population */
} /* private namespace geneial */
