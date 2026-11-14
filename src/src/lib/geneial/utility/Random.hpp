#pragma once

#include <geneial/utility/Random.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(utility)
    {
        geneial_export_namespace
        {

            template <typename T> T Random::generateRange(T min, T max, std::true_type)
            {
                using DistributionType =
                    std::conditional_t<(sizeof(T) < sizeof(int)),
                                       std::conditional_t<std::is_signed_v<T>, int, unsigned int>, T>;
                return static_cast<T>(std::uniform_int_distribution<DistributionType>(min, max)(engine()));
            }

            template <typename T> T Random::generateRange(T min, T max, std::false_type)
            {
                static_assert(std::is_floating_point<T>::value, "Random requires a numeric type");
                if (!std::isfinite(min) || !std::isfinite(max))
                {
                    throw std::invalid_argument("Random bounds must be finite");
                }
                if (min == max)
                {
                    return min;
                }
                const T fraction = std::uniform_real_distribution<T>(0, 1)(engine());
                const T value =
                    min < 0 && max > 0 ? min * (1 - fraction) + max * fraction : min + (max - min) * fraction;
                return std::max(min, std::min(std::nextafter(max, min), value));
            }

            template <typename T> T Random::generate(const T min, const T max)
            {
                if (!(min <= max))
                {
                    throw std::invalid_argument("Random minimum must not exceed maximum");
                }
                return generateRange(min, max, std::is_integral<T>());
            }

            template <typename T> T Random::generate()
            {
                return generate<T>(0, std::is_integral<T>::value ? std::numeric_limits<T>::max() : T(1));
            }

            inline bool Random::generateBit()
            {
                return std::bernoulli_distribution(0.5)(engine());
            }

            inline bool Random::decision(const double probability)
            {
                if (!(probability >= 0 && probability <= 1))
                {
                    throw std::invalid_argument("Probability must be in [0, 1]");
                }
                return std::bernoulli_distribution(probability)(engine());
            }
        }
    }
}
