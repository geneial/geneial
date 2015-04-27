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

    SelectionSettings(unsigned int numberOfParents) :
            _numberOfParents(numberOfParents)
    {
        assert(_numberOfParents > 0);
    }

};

} /* namespace selection */
} /* namespace operation */
} /* namespace geneial */

