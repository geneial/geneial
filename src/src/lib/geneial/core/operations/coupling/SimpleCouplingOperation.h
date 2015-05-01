#pragma once

#include <geneial/core/operations/coupling/BaseCouplingOperation.h>

namespace geneial
{
namespace operation
{
namespace coupling
{

/**
 * SimpleCouplingOperation iterates over the mating pool and takes the first two partners, crosses them, and produces the given offspring.
 * A wrap around is in the mating pool is performed, if the coupling settings requires more offspring than required.
 */
template<typename FITNESS_TYPE>
class SimpleCouplingOperation: public BaseCouplingOperation<FITNESS_TYPE>
{
public:
    explicit SimpleCouplingOperation(const CouplingSettings &settings) :
            BaseCouplingOperation<FITNESS_TYPE>(settings)
    {
    }

    virtual ~SimpleCouplingOperation()
    {
    }

    virtual typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set doCopulate(
            const typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &mating_pool,
            const BaseCrossoverOperation<FITNESS_TYPE> &crossoverOperation, BaseManager<FITNESS_TYPE> &manager) override;

};

} /* namespace coupling */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/coupling/SimpleCouplingOperation.hpp>

