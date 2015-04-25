#ifndef __GENEIAL_COUPLING_SETTINGS_H_
#define __GENEIAL_COUPLING_SETTINGS_H_

#include <cassert>

namespace geneial
{
namespace operation
{
namespace coupling
{

/**
 * Generic class for coupling options
 */
class CouplingSettings
{
private:
    unsigned int _numberOfOffspring;
public:
    virtual ~CouplingSettings()
    {
    }

    CouplingSettings(unsigned int numberOfOffspring) :
            _numberOfOffspring(numberOfOffspring)
    {
        assert(numberOfOffspring > 0);
    }

    unsigned int getNumberOfOffspring() const
    {
        return _numberOfOffspring;
    }

    void setNumberOfOffspring(unsigned int numberOfOffspring)
    {
        _numberOfOffspring = numberOfOffspring;
    }

};

} /* namespace coupling */
} /* namespace operation */
} /* namespace geneial */

#endif /* __GENEIAL_COUPLING_SETTINGS_H_ */
