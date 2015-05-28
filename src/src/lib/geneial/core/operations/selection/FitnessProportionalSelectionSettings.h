#pragma once

#include <cassert>
#include <geneial/namespaces.h>
#include <geneial/core/operations/selection/BaseSelectionSettings.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(selection)
{

geneial_export_namespace
{

/**
 * Select a number of parents based on a certain criteria.
 */
class FitnessProportionalSelectionSettings: public BaseSelectionSettings
{
private:
    unsigned int _numberSelectBest;

public:
    const constexpr static unsigned int DEFAULT_NUMBER_SELECT_BEST = 1;

    FitnessProportionalSelectionSettings(
            unsigned int numberOfParents = BaseSelectionSettings::DEFAULT_NUMBER_OF_PARENTS,
            unsigned int numberSelectBest = DEFAULT_NUMBER_SELECT_BEST) :
            BaseSelectionSettings(numberOfParents), _numberSelectBest(numberSelectBest)
    {
        assert(numberOfParents >= numberSelectBest);
    }

    unsigned int getNumberSelectBest() const
    {
        return _numberSelectBest;
    }

    void setNumberSelectBest(unsigned int numberSelectBest)
    {
        _numberSelectBest = numberSelectBest;
    }

};

} /* geneial_export_namespace */
} /* private namespace selection */
} /* private namespace operation */
} /* private namespace geneial */

