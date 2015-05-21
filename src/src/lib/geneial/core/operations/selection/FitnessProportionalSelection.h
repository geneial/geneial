#pragma once

#include <geneial/core/operations/selection/BaseSelectionOperation.h>
#include <geneial/core/operations/selection/FitnessProportionalSelectionSettings.h>

#include <stdexcept>
#include <memory>

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
    explicit FitnessProportionalSelection(const std::shared_ptr<const FitnessProportionalSelectionSettings>& settings) :
            BaseSelectionOperation<FITNESS_TYPE>(settings), _settings(settings)
    {
    }

    virtual ~FitnessProportionalSelection()
    {
    }

    virtual typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set doSelect(
            const Population<FITNESS_TYPE> &population, BaseManager<FITNESS_TYPE> &manager) const override;

private:
    const std::shared_ptr<const FitnessProportionalSelectionSettings> _settings;

};

} /* namespace selection */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/selection/FitnessProportionalSelection.hpp>

