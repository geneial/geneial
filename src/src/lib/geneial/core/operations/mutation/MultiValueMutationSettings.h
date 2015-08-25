#pragma once

#include <geneial/namespaces.h>

#include <cassert>


geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(mutation)
{
geneial_export_namespace
{
class MultiValueMutationSettings
{
private:
    unsigned int _minimumPointsToMutate;

    unsigned int _maximumPointsToMutate;

public:
    static const unsigned int DEFAULT_MIN_AMOUNT_OF_POINTS = 1;
    static const unsigned int DEFAULT_MAX_AMOUNT_OF_POINTS = 5;

    virtual ~MultiValueMutationSettings()
    {
    }

    MultiValueMutationSettings( const unsigned int minimumPointsToMutate = DEFAULT_MIN_AMOUNT_OF_POINTS,
                                const unsigned int maximumPointsToMutate = DEFAULT_MAX_AMOUNT_OF_POINTS)
    :_minimumPointsToMutate(minimumPointsToMutate),
     _maximumPointsToMutate(maximumPointsToMutate)
    {
    }

    unsigned int getMaximumPointsToMutate() const
    {
        return _maximumPointsToMutate;
    }

    void setMaximumPointsToMutate(unsigned int maximumPointsToMutate)
    {
        _maximumPointsToMutate = maximumPointsToMutate;
    }

    unsigned int getMinimumPointsToMutate() const
    {
        return _minimumPointsToMutate;
    }

    void setMinimumPointsToMutate(unsigned int minimumPointsToMutate)
    {
        _minimumPointsToMutate = minimumPointsToMutate;
    }
};
} /* geneial_export_namespace */
} /* private namespace mutation */
} /* private namespace operation */
} /* private namespace geneial */

