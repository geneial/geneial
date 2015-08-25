#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{
using ::geneial::population::chromosome::MultiValueChromosome;

geneial_export_namespace
{
//Library for some common smoothing operations

class Smoothing
{

private:
    Smoothing()
    {
    }

    virtual ~Smoothing()
    {
    }

public:

    //TODO (bewo): Docs!
    template<typename VALUE_TYPE, typename FITNESS_TYPE>
    static void restoreSmoothness(typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr chromosome,
            VALUE_TYPE maxAbsElevation, VALUE_TYPE min, VALUE_TYPE max, bool hasStart, VALUE_TYPE start);

    //TODO (bewo): Docs!
    template<typename VALUE_TYPE, typename FITNESS_TYPE>
    static void peakAt(unsigned int pos, unsigned int epsLeft, unsigned int epsRight, VALUE_TYPE elevation,
            typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr chromosome);

};

} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */

#include <geneial/utility/Smoothing.hpp>
