#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/selection/BaseSelectionSettings.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/core/population/Population.h>
#include <geneial/utility/mixins/Buildable.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(selection)
{
using ::geneial::population::Population;
using ::geneial::population::management::BaseManager;
using ::geneial::utility::Buildable;

geneial_export_namespace
{

/**
 * Select a number of parents based on a certain criteria.
 */
template<typename FITNESS_TYPE>
class BaseSelectionOperation : public Buildable<BaseSelectionOperation<FITNESS_TYPE>>
{
private:
    const std::shared_ptr<BaseSelectionSettings> _settings;


protected:
    BaseSelectionOperation(const std::shared_ptr<BaseSelectionSettings> &settings) :
            _settings(settings)
    {
    }

public:
    using selection_result_set = typename Population<FITNESS_TYPE>::chromosome_container;

    virtual ~BaseSelectionOperation()
    {
    }

    virtual selection_result_set doSelect(const Population<FITNESS_TYPE> &population,
            BaseManager<FITNESS_TYPE> &manager) const = 0;


    const BaseSelectionSettings& getSettings() const
    {
        return *_settings;
    }

    void setSettings(const std::shared_ptr<BaseSelectionSettings> &settings)
    {
        _settings = settings;
    }

    class Builder: Buildable<BaseSelectionOperation<FITNESS_TYPE>>::Builder
    {
    protected:
        std::shared_ptr<BaseSelectionSettings> _settings;
    public:
        Builder() :
                _settings(new BaseSelectionSettings)
        {
        }

        Builder(const std::shared_ptr<BaseSelectionSettings>&settings) :
                _settings(settings)
        {
        }

        BaseSelectionSettings& getSettings()
        {
            return *_settings;
        }
    };
};

} /* geneial_export_namespace */
} /* private namespace selection */
} /* private namespace operation */
} /* private namespace geneial */

