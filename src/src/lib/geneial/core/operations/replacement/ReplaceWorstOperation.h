#pragma once

#include <geneial/core/operations/replacement/BaseReplacementOperation.h>

namespace geneial
{
namespace operation
{
namespace replacement
{

/**
 * Select a number of parents based on a certain criteria.
 */
template<typename FITNESS_TYPE>
class ReplaceWorstOperation: public BaseReplacementOperation<FITNESS_TYPE>
{
private:
    unsigned int getAmountToReplace(const Population<FITNESS_TYPE> &population,
            const typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring) const;
public:
    ReplaceWorstOperation(const BaseReplacementSettings& settings) :
            BaseReplacementOperation<FITNESS_TYPE>(settings)
    {
    }

    virtual ~ReplaceWorstOperation()
    {
    }

    void doReplace(Population<FITNESS_TYPE> &population,
            const typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &parents,
            typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring,
            BaseManager<FITNESS_TYPE> &manager) override;

};

} /* namespace replacement */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/replacement/ReplaceWorstOperation.hpp>

