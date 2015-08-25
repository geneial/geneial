#pragma once

#include <geneial/core/operations/replacement/BaseReplacementOperation.h>
#include <geneial/utility/mixins/EnableMakeShared.h>


geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(replacement)
{
using ::geneial::operation::coupling::BaseCouplingOperation;
using ::geneial::operation::selection::BaseSelectionOperation;
using ::geneial::utility::EnableMakeShared;

geneial_export_namespace
{

/**
 * Replace chromosomes at random in the population
 */
template<typename FITNESS_TYPE>
class ReplaceRandomOperation:   public BaseReplacementOperation<FITNESS_TYPE>,
                                public EnableMakeShared<ReplaceRandomOperation<FITNESS_TYPE>>
{
private:
    unsigned int getAmountToReplace(const Population<FITNESS_TYPE> &population,
            const typename coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring) const;

protected:
    explicit ReplaceRandomOperation(const std::shared_ptr<const BaseReplacementSettings> &settings) :
            BaseReplacementOperation<FITNESS_TYPE>(settings)
    {
    }

public:

    virtual ~ReplaceRandomOperation()
    {
    }

    void doReplace(Population<FITNESS_TYPE> &population,
            const typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &parents,
            typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring,
            BaseManager<FITNESS_TYPE> &manager) const override;

    class Builder : public BaseReplacementOperation<FITNESS_TYPE>::Builder
    {
    public:
        Builder():BaseReplacementOperation<FITNESS_TYPE>::Builder()
        {
        }
        typename BaseReplacementOperation<FITNESS_TYPE>::ptr create() override
        {
            return std::move(
                        ReplaceRandomOperation<FITNESS_TYPE>::makeShared(this->_settings)
                    );
        }
    };
};

} /* geneial_export_namespace */
} /* private namespace replacement */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/replacement/ReplaceRandomOperation.hpp>

