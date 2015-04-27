#pragma once

#include <geneial/core/operations/selection/SelectionSettings.h>

namespace geneial
{
namespace operation
{
namespace selection
{

using namespace geneial::population::management;

/**
 * Select a number of parents based on a certain criteria.
 */
template<typename FITNESS_TYPE>
class BaseSelectionOperation
{
private:
    const SelectionSettings& _settings;

public:
    typedef typename Population<FITNESS_TYPE>::chromosome_container selection_result_set;

    BaseSelectionOperation(SelectionSettings const & settings) :
            _settings(settings)
    {
    }

    virtual ~BaseSelectionOperation()
    {
    }

    virtual selection_result_set doSelect(const Population<FITNESS_TYPE> &population,
            BaseManager<FITNESS_TYPE> &manager) = 0;

    const SelectionSettings& getSettings() const
    {
        return _settings;
    }

    void setSettings(const SelectionSettings& settings)
    {
        _settings = settings;
    }

};

} /* namespace selection */
} /* namespace operation */
} /* namespace geneial */

