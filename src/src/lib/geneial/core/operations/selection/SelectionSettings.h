/*
 * BaseSelectionOperation.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef SELECTION_SETTINGS_H_
#define SELECTION_SETTINGS_H_

#include "assert.h"
namespace GeneticLibrary {
namespace Operation {
namespace Selection {
/**
 * Select a number of parents based on a certain criteria.
 */
class SelectionSettings{
public:
	virtual ~SelectionSettings(){};

	unsigned int getNumberOfParents() const {
		return _numberOfParents;
	}

	void setNumberOfParents(unsigned int numberOfParents) {
		_numberOfParents = numberOfParents;
	}

	SelectionSettings(unsigned int numberOfParents):
		_numberOfParents(numberOfParents){
		assert(_numberOfParents>0);
	};

private:
	unsigned int _numberOfParents;

};

} /* namespace Selection */
} /* namespace Operation */
} /* namespace GeneticLibrary */



#endif /* SELECTION_SETTINGS_H_ */
