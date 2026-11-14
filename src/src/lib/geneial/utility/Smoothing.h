#pragma once

#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <geneial/namespaces.h>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(utility)
    {
        using ::geneial::population::chromosome::MultiValueChromosome;

        geneial_export_namespace
        {
            // Library for some common smoothing operations

            class Smoothing
            {

              private:
                Smoothing() {}

                virtual ~Smoothing() {}

              public:
                /** Clamps genes to bounds and adjacent-step limits; an optional anchor constrains the first step. */
                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                static void restoreSmoothness(typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr chromosome,
                                              VALUE_TYPE maxAbsElevation, VALUE_TYPE min, VALUE_TYPE max, bool hasStart,
                                              VALUE_TYPE start);

                /** Adds a linearly tapered peak, clipping at genome edges and saturating the numeric value type. */
                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                static void peakAt(unsigned int pos, unsigned int epsLeft, unsigned int epsRight, VALUE_TYPE elevation,
                                   typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr chromosome);
            };

        } /* geneial_export_namespace */
    } /* private namespace utility */
} /* private namespace geneial */

#include <geneial/utility/Smoothing.hpp>
