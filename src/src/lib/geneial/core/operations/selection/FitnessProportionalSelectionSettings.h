#pragma once

#include <cassert>

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

} /* geneial_export_namespace */
} /* private namespace selection */
} /* private namespace operation */
} /* private namespace geneial */


