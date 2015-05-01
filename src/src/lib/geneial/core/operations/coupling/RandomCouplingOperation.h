#pragma once

#include <geneial/core/operations/coupling/BaseCouplingOperation.h>

namespace geneial
{
namespace operation
{
namespace coupling
{

using namespace geneial::operation::selection;
using namespace geneial::population::management;

/**
 * RandomCouplingOperation will choose parents from the mating pool at random
 */
template<typename FITNESS_TYPE>
class RandomCouplingOperation: public BaseCouplingOperation<FITNESS_TYPE>
{

public:
    explicit RandomCouplingOperation(const CouplingSettings &settings) :
            BaseCouplingOperation<FITNESS_TYPE>(settings)
    {
    }

    virtual ~RandomCouplingOperation()
    {
    }

    virtual typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set doCopulate(
            const typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &mating_pool,
            const BaseCrossoverOperation<FITNESS_TYPE> &crossoverOperation, BaseManager<FITNESS_TYPE> &manager) override;

};

} /* namespace coupling */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/coupling/RandomCouplingOperation.hpp>

