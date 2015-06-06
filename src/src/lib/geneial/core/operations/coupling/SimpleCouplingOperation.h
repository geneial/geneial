#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/coupling/BaseCouplingOperation.h>
#include <geneial/utility/mixins/EnableMakeShared.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(coupling)
{
using ::geneial::population::Population;
using ::geneial::operation::crossover::BaseCrossoverOperation;
using ::geneial::operation::selection::BaseSelectionOperation;
using ::geneial::population::management::BaseManager;
using ::geneial::utility::EnableMakeShared;

geneial_export_namespace
{

/**
 * SimpleCouplingOperation iterates over the mating pool and takes the first two partners, crosses them, and produces the given offspring.
 * A wrap around is in the mating pool is performed, if the coupling settings requires more offspring than required.
 */
template<typename FITNESS_TYPE>
class SimpleCouplingOperation: public BaseCouplingOperation<FITNESS_TYPE>,
                               public virtual EnableMakeShared<SimpleCouplingOperation<FITNESS_TYPE>>
{
protected:
    explicit SimpleCouplingOperation( const std::shared_ptr<const CouplingSettings> &settings) :
            BaseCouplingOperation<FITNESS_TYPE>(settings)
    {
    }

public:
    virtual ~SimpleCouplingOperation()
    {
    }

    virtual typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set doCopulate(
            const typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &mating_pool,
            const BaseCrossoverOperation<FITNESS_TYPE> &crossoverOperation, BaseManager<FITNESS_TYPE> &manager) override;

    class Builder:public BaseCouplingOperation<FITNESS_TYPE>::Builder
    {
    public:
        virtual typename BaseCouplingOperation<FITNESS_TYPE>::ptr create() override
        {
            return SimpleCouplingOperation::makeShared(this->_settings);
        }
    };

};

} /* geneial_export_namespace */
} /* private namespace coupling */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/coupling/SimpleCouplingOperation.hpp>

