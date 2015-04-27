#pragma once

#include <cassert>

namespace geneial
{
namespace operation
{
namespace selection
{

/**
 * Select a number of parents based on a certain criteria.
 */
class FitnessProportionalSelectionSettings: public SelectionSettings
{
private:
    unsigned int _numberSelectBest;

public:
    FitnessProportionalSelectionSettings(unsigned int numberOfParents, unsigned int numberSelectBest) :
            SelectionSettings(numberOfParents), _numberSelectBest(numberSelectBest)
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

} /* namespace selection */
} /* namespace operation */
} /* namespace geneial */

