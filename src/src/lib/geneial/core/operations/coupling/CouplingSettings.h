/*
 * CouplingSettings.h
 *
 *  Created on: Dec 12, 2014
 *      Author: bewo
 */

#ifndef COUPLINGSETTINGS_H_
#define COUPLINGSETTINGS_H_

#include <cassert>

namespace GeneticLibrary {
namespace Operation {
namespace Coupling {
/**
 * Generic class for coupling options
 */
class CouplingSettings{
private:
	unsigned int _numberOfOffspring;
public:
	virtual ~CouplingSettings(){};

	CouplingSettings(unsigned int numberOfOffspring):
		_numberOfOffspring(numberOfOffspring)
	{
		assert(numberOfOffspring >0);
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

} /* namespace Coupling */
} /* namespace Operation */
} /* namespace GeneticLibrary */



#endif /* COUPLINGSETTINGS_H_ */
