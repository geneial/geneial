/*
 * BaseSelectionOperation.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef BASEREPLACEMENTOPERATION_H_
#define BASEREPLACEMENTOPERATION_H_

#include <geneial/core/operations/replacement/BaseReplacementSettings.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Operation {
namespace Replacement {
/**
 * Select a number of parents based on a certain criteria.
 */
template <typename FITNESS_TYPE>
class BaseReplacementOperation{
private:
	BaseReplacementSettings * _settings;

public:
	BaseReplacementOperation(BaseReplacementSettings* settings): _settings(settings) {};

	virtual ~BaseReplacementOperation() {};

	virtual void doReplace(
			typename Population::Population<FITNESS_TYPE> &population,
			typename Selection::BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &parents,
			typename Coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring,
			BaseManager<FITNESS_TYPE> &manager) = 0;


	BaseReplacementSettings* getSettings() const {
		return _settings;
	}

	void setSettings(BaseReplacementSettings* settings) {
		_settings = settings;
	}

};

} /* namespace Replacement */
} /* namespace Operation */
} /* namespace GeneticLibrary */



#endif /* BASEREPLACEMENTOPERATION_H_ */
