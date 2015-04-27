#pragma once

#include <geneial/core/population/chromosome/MultiValueChromosome.h>

namespace geneial
{
namespace utility
{

//Library for some common smoothing operations

class Smoothing
{

private:
    Smoothing()
    {
    }
    ;
    virtual ~Smoothing()
    {
    }
    ;

public:

    //TODO (bewo): Docs!
    template<typename VALUE_TYPE, typename FITNESS_TYPE>
    static void restoreSmoothness(typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr chromosome,
            VALUE_TYPE maxAbsElevation, VALUE_TYPE min, VALUE_TYPE max);

    //TODO (bewo): Docs!
    template<typename VALUE_TYPE, typename FITNESS_TYPE>
    static void peakAt(unsigned int pos, unsigned int epsLeft, unsigned int epsRight, VALUE_TYPE elevation,
            typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr chromosome);

};

} /* namespace utility */
} /* namespace geneial */

#include <geneial/utility/Smoothing.hpp>

