#pragma once

#include <vector>

namespace geneial
{
namespace population
{

template<typename FITNESS_TYPE>
class ContainerTypes
{
private:
    ContainerTypes()
    {
    }

public:
    //A generic container to pass chromomsomes between operations.
    typedef typename std::vector<typename chromosome::BaseChromosome<FITNESS_TYPE>::ptr> chromosome_container;
};

} /* namespace population */
} /* namespace geneial */

