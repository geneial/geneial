#pragma once

#include <geneial/core/operations/selection/BaseSelectionOperation.h>
#include <geneial/core/operations/selection/FitnessProportionalSelectionSettings.h>
#include <geneial/utility/mixins/EnableMakeShared.h>

#include <stdexcept>
#include <memory>

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
 * Select a number of parents based on a certain criteria.
 */
template<typename FITNESS_TYPE>
class FitnessProportionalSelection: public BaseSelectionOperation<FITNESS_TYPE>,
                                    public virtual EnableMakeShared<FitnessProportionalSelection<FITNESS_TYPE>>
{
private:
    //TODO(bewo) this is ugly since it hides the fact that there are base class settings.
    const std::shared_ptr<FitnessProportionalSelectionSettings> _settings;

protected:
    FitnessProportionalSelection(const std::shared_ptr<FitnessProportionalSelectionSettings>& settings) :
            BaseSelectionOperation<FITNESS_TYPE>(settings), _settings(settings)
    {
    }

public:

    virtual ~FitnessProportionalSelection()
    {
    }

    virtual typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set doSelect(
            const Population<FITNESS_TYPE> &population, BaseManager<FITNESS_TYPE> &manager) const override;


    class Builder : public BaseSelectionOperation<FITNESS_TYPE>::Builder
    {

    public:
        Builder() : BaseSelectionOperation<FITNESS_TYPE>::Builder(std::make_shared<FitnessProportionalSelectionSettings>())
        {
        }

        FitnessProportionalSelectionSettings& getSettings()
        {
            return *std::dynamic_pointer_cast<FitnessProportionalSelectionSettings>(this->_settings);
        }

        typename BaseSelectionOperation<FITNESS_TYPE>::ptr create() override
        {
            auto settings = std::dynamic_pointer_cast<FitnessProportionalSelectionSettings>(this->_settings);
            return FitnessProportionalSelection<FITNESS_TYPE>::makeShared(settings);
        }

    };

};

} /* geneial_export_namespace */
} /* private namespace selection */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/selection/FitnessProportionalSelection.hpp>

