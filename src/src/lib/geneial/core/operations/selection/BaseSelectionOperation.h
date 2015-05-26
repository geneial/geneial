#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/selection/SelectionSettings.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/core/population/Population.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(selection)
{
using ::geneial::population::Population;
using ::geneial::population::management::BaseManager;

geneial_export_namespace
{

/**
 * Select a number of parents based on a certain criteria.
 */
template<typename FITNESS_TYPE>
class BaseSelectionOperation
{
private:
    const std::shared_ptr<const SelectionSettings> _settings;

public:
    typedef typename Population<FITNESS_TYPE>::chromosome_container selection_result_set;

    explicit BaseSelectionOperation(const std::shared_ptr<const SelectionSettings> &settings) :
            _settings(settings)
    {
    }

    virtual ~BaseSelectionOperation()
    {
    }

    virtual selection_result_set doSelect(const Population<FITNESS_TYPE> &population,
            BaseManager<FITNESS_TYPE> &manager) const = 0;

    const SelectionSettings& getSettings() const
    {
        return *_settings;
    }

    void setSettings(const std::shared_ptr<const SelectionSettings> &settings)
    {
        _settings = settings;
    }

};

} /* geneial_export_namespace */
} /* private namespace selection */
} /* private namespace operation */
} /* private namespace geneial */

