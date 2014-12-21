/*
 * BaseSelectionOperation.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef BASESELECTIONOPERATION_H_
#define BASESELECTIONOPERATION_H_

#include <geneial/core/operations/selection/SelectionSettings.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Operation {
namespace Selection {
/**
 * Select a number of parents based on a certain criteria.
 */
template <typename FITNESS_TYPE>
class BaseSelectionOperation{
private:
	SelectionSettings* _settings;

public:
	typedef typename Population::Population<FITNESS_TYPE>::chromosome_container selection_result_set;

	BaseSelectionOperation(SelectionSettings* settings): _settings(settings) {};

	virtual ~BaseSelectionOperation() {};

	virtual selection_result_set doSelect( const Population::Population<FITNESS_TYPE> &population, BaseManager<FITNESS_TYPE> &manager) = 0;

	SelectionSettings* getSettings() const {
		return _settings;
	}

	void setSettings(SelectionSettings* settings) {
		_settings = settings;
	}

};

} /* namespace Selection */
} /* namespace Operation */
} /* namespace GeneticLibrary */



#endif /* BASESELECTIONOPERATION_H_ */
