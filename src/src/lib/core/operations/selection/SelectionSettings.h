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
	unsigned int getElitismSuccessors() const {
		return _elitismSuccessors;
	}

	void setElitismSuccessors(unsigned int elitismSuccessors) {
		_elitismSuccessors = elitismSuccessors;
	}

	unsigned int getNumberOfParents() const {
		return _numberOfParents;
	}

	void setNumberOfParents(unsigned int numberOfParents) {
		_numberOfParents = numberOfParents;
	}

	SelectionSettings(unsigned int numberOfParents):
		_numberOfParents(numberOfParents),
		_elitismSuccessors(0){
		assert(_numberOfParents>=_elitismSuccessors);
	};
	SelectionSettings(unsigned int numberOfParents,unsigned int elitismSuccessors):
		_numberOfParents(numberOfParents)
		,_elitismSuccessors(elitismSuccessors){
		assert(_numberOfParents>=_elitismSuccessors);
	};

private:
	unsigned int _numberOfParents;
	unsigned int _elitismSuccessors;

};

} /* namespace Selection */
} /* namespace Operation */
} /* namespace GeneticLibrary */



#endif /* SELECTION_SETTINGS_H_ */
