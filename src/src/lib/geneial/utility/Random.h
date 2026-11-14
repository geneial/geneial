#pragma once

#include <algorithm>
#include <cstdint>
#include <geneial/namespaces.h>
#include <random>
#include <type_traits>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(utility)
    {

        geneial_export_namespace
        {

            class Random
            {
                Random() = delete;
                Random(const Random &) = delete;

              public:
                static inline bool generateBit();

                static inline bool decision(const double probability);

                template <typename T> static inline T generate(const T min, const T max);

                template <typename T> static inline T generate();

                virtual ~Random() {}

                static void initialize();
                /** Resets only the calling thread's engine; distribution sequences are standard-library dependent. */
                static void initialize(std::uint32_t seed);

                template <typename Iterator> static void shuffle(Iterator first, Iterator last)
                {
                    std::shuffle(first, last, engine());
                }

              private:
                static std::mt19937 &engine();

                template <typename T> static T generateRange(T min, T max, std::true_type);

                template <typename T> static T generateRange(T min, T max, std::false_type);
            };

        } /* geneial_export_namespace */
    } /* private namespace utility */
} /* private namespace geneial */

#include <geneial/utility/Random.hpp>
