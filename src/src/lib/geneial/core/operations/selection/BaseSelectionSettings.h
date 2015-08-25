#pragma once

#include <geneial/namespaces.h>

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
class BaseSelectionSettings
{
private:
    unsigned int _numberOfParents;

public:
    static const constexpr unsigned int DEFAULT_NUMBER_OF_PARENTS = 5;

    virtual ~BaseSelectionSettings()
    {
    }

    unsigned int getNumberOfParents() const
    {
        return _numberOfParents;
    }

    void setNumberOfParents(const unsigned int numberOfParents)
    {
        _numberOfParents = numberOfParents;
    }

    explicit BaseSelectionSettings(const unsigned int numberOfParents = DEFAULT_NUMBER_OF_PARENTS) :
            _numberOfParents(numberOfParents)
    {
        assert(_numberOfParents > 0);
    }

};

} /* geneial_export_namespace */
} /* private namespace selection */
} /* private namespace operation */
} /* private namespace geneial */

