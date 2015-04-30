#pragma once

#include <geneial/core/operations/coupling/CouplingSettings.h>
#include <geneial/core/operations/selection/BaseSelectionOperation.h>
#include <geneial/core/operations/crossover/BaseCrossoverOperation.h>
#include <geneial/core/population/management/BaseManager.h>

namespace geneial
{
namespace operation
{
namespace coupling
{

using namespace geneial::operation::selection;
using namespace geneial::operation::crossover;
using namespace geneial::population::management;

template<typename FITNESS_TYPE>
class BaseCouplingOperation
{

private:
    const CouplingSettings& _settings;

public:
    typedef typename Population<FITNESS_TYPE>::chromosome_container offspring_result_set;

    BaseCouplingOperation(const CouplingSettings &settings) :
            _settings(settings)
    {
    }

    virtual ~BaseCouplingOperation()
    {
    }

    virtual offspring_result_set doCopulate(
            const typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &mating_pool,
            const BaseCrossoverOperation<FITNESS_TYPE> &crossoverOperation, BaseManager<FITNESS_TYPE> &manager) = 0;

    CouplingSettings const & getSettings() const
    {
        return _settings;
    }

    void setSettings(const CouplingSettings& settings)
    {
        _settings = settings;
    }

protected:
   typename offspring_result_set::size_type copyUnlessMaximumReached(
            offspring_result_set &dest,
            const typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set &input,
            const typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set::size_type limit) const;

};

} /* namespace coupling */
} /* namespace operation */
} /* namespace geneial */


#include <geneial/core/operations/coupling/BaseCouplingOperation.hpp>
