#pragma once

#include <geneial/core/operations/selection/BaseSelectionOperation.h>

#include <map>
#include <stdexcept>
#include <memory>

namespace geneial
{
namespace operation
{
namespace selection
{

/**
 * Select a number of parents based on a roulette wheel distribution over the chromosomes fitness
 */
template<typename FITNESS_TYPE>
class RouletteWheelSelection: public BaseSelectionOperation<FITNESS_TYPE>
{
public:
    explicit RouletteWheelSelection(const std::shared_ptr<const SelectionSettings> &settings) :
            BaseSelectionOperation<FITNESS_TYPE>(settings)
    {
    }

    virtual ~RouletteWheelSelection()
    {
    }

    virtual typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set doSelect(
            const Population<FITNESS_TYPE> &population, BaseManager<FITNESS_TYPE> &manager) const override;

};

} /* namespace selection */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/selection/RouletteWheelSelection.hpp>

