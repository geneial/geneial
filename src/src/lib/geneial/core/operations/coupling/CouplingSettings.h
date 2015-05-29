#pragma once

#include <geneial/namespaces.h>

#include <cassert>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(coupling)
{
geneial_export_namespace
{
/**
 * Generic class for coupling options
 */
class CouplingSettings
{
private:
    unsigned int _numberOfOffspring;
public:
    const constexpr static unsigned int DEFAULT_NUMBER_OF_OFFSPRING = 20;

    virtual ~CouplingSettings()
    {
    }

    explicit CouplingSettings(const unsigned int numberOfOffspring = DEFAULT_NUMBER_OF_OFFSPRING) :
            _numberOfOffspring(numberOfOffspring)
    {
        assert(numberOfOffspring > 0);
    }

    unsigned int getNumberOfOffspring() const
    {
        return _numberOfOffspring;
    }

    void setNumberOfOffspring(const unsigned int numberOfOffspring)
    {
        _numberOfOffspring = numberOfOffspring;
    }

};

} /* geneial_export_namespace */
} /* private namespace coupling */
} /* private namespace operation */
} /* private namespace geneial */

