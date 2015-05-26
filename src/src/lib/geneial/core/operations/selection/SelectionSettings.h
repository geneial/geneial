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
class SelectionSettings
{
private:
    unsigned int _numberOfParents;

public:
    virtual ~SelectionSettings()
    {
    }

    unsigned int getNumberOfParents() const
    {
        return _numberOfParents;
    }

    void setNumberOfParents(unsigned int numberOfParents)
    {
        _numberOfParents = numberOfParents;
    }

    explicit SelectionSettings(unsigned int numberOfParents) :
            _numberOfParents(numberOfParents)
    {
        assert(_numberOfParents > 0);
    }

};

} /* geneial_export_namespace */
} /* private namespace selection */
} /* private namespace operation */
} /* private namespace geneial */

