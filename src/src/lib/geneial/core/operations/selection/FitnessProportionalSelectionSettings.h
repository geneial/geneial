/*
 * BaseSelectionOperation.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef FITNESS_PROP_SELECTION_SETTINGS_H_
#define FITNESS_PROP_SELECTION_SETTINGS_H_

#include "assert.h"
namespace GeneticLibrary {
namespace Operation {
namespace Selection {
/**
 * Select a number of parents based on a certain criteria.
 */
class FitnessProportionalSelectionSettings: public SelectionSettings{
public:
	FitnessProportionalSelectionSettings(unsigned int numberOfParents, unsigned int numberSelectBest):
		SelectionSettings(numberOfParents),
		_numberSelectBest(numberSelectBest) {
			assert(numberOfParents>=numberSelectBest);
	}
	unsigned int getNumberSelectBest() const {
		return _numberSelectBest;
	}

	void setNumberSelectBest(unsigned int numberSelectBest) {
		_numberSelectBest = numberSelectBest;
	}

	;

private:
	unsigned int _numberSelectBest;

};

} /* namespace Selection */
} /* namespace Operation */
} /* namespace GeneticLibrary */



#endif /* FITNESS_PROP_SELECTION_SETTINGS_H_ */
