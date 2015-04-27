#pragma once

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
    doCrossover(typename BaseChromosome<FITNESS_TYPE>::ptr mommy, typename BaseChromosome<FITNESS_TYPE>::ptr daddy) = 0;

    //Is the order of mommy for and daddy for doCrossover() relevant?, true if yes.
    virtual bool inline isSymmetric() const = 0;

};

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */

