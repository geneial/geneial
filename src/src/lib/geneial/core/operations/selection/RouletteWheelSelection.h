#pragma once

#include <geneial/core/operations/selection/BaseSelectionOperation.h>

#include <map>
#include <stdexcept>
#include <memory>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(selection)
{
geneial_export_namespace
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

} /* geneial_export_namespace */
} /* private namespace selection */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/selection/RouletteWheelSelection.hpp>

