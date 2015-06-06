#pragma once

#include <geneial/core/operations/selection/BaseSelectionOperation.h>
#include <geneial/utility/mixins/EnableMakeShared.h>

#include <map>
#include <stdexcept>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(selection)
{
using ::geneial::utility::EnableMakeShared;

geneial_export_namespace
{

/**
 * Select a number of parents based on a roulette wheel distribution over the chromosomes fitness
 */
template<typename FITNESS_TYPE>
class UniformRandomSelection: public BaseSelectionOperation<FITNESS_TYPE>,
                              public virtual EnableMakeShared<UniformRandomSelection<FITNESS_TYPE>>
{
protected:
    UniformRandomSelection(const std::shared_ptr<BaseSelectionSettings> &settings) :
            BaseSelectionOperation<FITNESS_TYPE>(settings)
    {
    }

    virtual ~UniformRandomSelection()
    {
    }
public:
    virtual typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set doSelect(
            const Population<FITNESS_TYPE> &population, BaseManager<FITNESS_TYPE> &manager) const override;

    class Builder : public BaseSelectionOperation<FITNESS_TYPE>::Builder
    {

    public:
        Builder() : BaseSelectionOperation<FITNESS_TYPE>::Builder()
        {
        }

        typename BaseSelectionOperation<FITNESS_TYPE>::ptr create() override
        {
            return UniformRandomSelection<FITNESS_TYPE>::makeShared(this->_settings);
        }

    };

};

} /* geneial_export_namespace */
} /* private namespace selection */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/selection/UniformRandomSelection.hpp>

