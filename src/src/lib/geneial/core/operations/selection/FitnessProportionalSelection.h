#pragma once

#include <geneial/core/operations/selection/BaseSelectionOperation.h>
#include <geneial/core/operations/selection/FitnessProportionalSelectionSettings.h>

#include <map>
#include <stdexcept>

namespace geneial
{
namespace operation
{
namespace selection
{

/**
 * Select a number of parents based on a certain criteria.
 */
template<typename FITNESS_TYPE>
class FitnessProportionalSelection: public BaseSelectionOperation<FITNESS_TYPE>
{
public:
    FitnessProportionalSelection(SelectionSettings* settings) :
            BaseSelectionOperation<FITNESS_TYPE>(settings)
    {
        _settings = dynamic_cast<FitnessProportionalSelectionSettings*>(settings);
        if (!_settings)
        {
            throw new std::runtime_error("settings must be FitnessProportionalSelectionSettings");
        }

    }

    virtual ~FitnessProportionalSelection()
    {
    }
    ;

    virtual typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set doSelect(
            const Population<FITNESS_TYPE> &population, BaseManager<FITNESS_TYPE> &manager);

private:
    FitnessProportionalSelectionSettings* _settings;

};

} /* namespace selection */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/selection/FitnessProportionalSelection.hpp>

