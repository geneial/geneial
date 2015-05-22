#pragma once

#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/core/population/Population.h>

namespace geneial
{
namespace operation
{
namespace crossover
{

using namespace geneial::population;
using namespace geneial::population::chromosome;

template<typename FITNESS_TYPE>
class BaseCrossoverOperation
{
public:
    typedef typename Population<FITNESS_TYPE>::chromosome_container crossover_result_set;
    BaseCrossoverOperation()
    {
    }

    virtual ~BaseCrossoverOperation()
    {
    }

    virtual crossover_result_set
    doCrossover(const typename BaseChromosome<FITNESS_TYPE>::const_ptr &mommy, const typename BaseChromosome<FITNESS_TYPE>::const_ptr &daddy) const = 0;

    //Is the order of mommy for and daddy for doCrossover() relevant?, true if yes.
    //TODO(bewo): Think about encoding this property via inheritance?
    virtual bool inline isSymmetric() const = 0;

};

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */

