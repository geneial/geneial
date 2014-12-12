/*
 * FitnessProportionalSelection.h
 *
 *  Created on: Dec 11, 2014
 *      Author: bewo
 */

#ifndef FITNESSPROPORTIONALSELECTION_H_
#define FITNESSPROPORTIONALSELECTION_H_

#include <geneial/core/operations/selection/BaseSelectionOperation.h>
#include <geneial/core/operations/selection/FitnessProportionalSelectionSettings.h>
#include <map>
#include <stdexcept>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Operation {
namespace Selection {
/**
 * Select a number of parents based on a certain criteria.
 */
template <typename FITNESS_TYPE>
class FitnessProportionalSelection : public BaseSelectionOperation<FITNESS_TYPE>{
private:
	FitnessProportionalSelectionSettings* _settings;
public:
	FitnessProportionalSelection(SelectionSettings* settings): BaseSelectionOperation<FITNESS_TYPE>(settings) {
		_settings = dynamic_cast<FitnessProportionalSelectionSettings*>(settings);
		if(!_settings){
			throw new runtime_error("settings must be FitnessProportionalSelectionSettings");
		}

	};
	virtual ~FitnessProportionalSelection(){};

	virtual typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set doSelect(
			const Population::Population<FITNESS_TYPE> &population,
			BaseManager<FITNESS_TYPE> &manager);



};


} /* namespace Selection */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#include <geneial/core/operations/selection/FitnessProportionalSelection.hpp>

#endif /* FITNESSPROPORTIONALSELECTION_H_ */
