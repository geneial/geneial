/*
 * FitnessProportionalSelection.h
 *
 *  Created on: Dec 11, 2014
 *      Author: bewo
 */

#ifndef ROULETTEWHEELSELECTION_H_
#define ROULETTEWHEELSELECTION_H_

#include <geneial/core/operations/selection/BaseSelectionOperation.h>

#include <map>
#include <stdexcept>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Operation {
namespace Selection {
/**
 * Select a number of parents based on a roulette wheel distribution over the chromosomes fitness
 */
template <typename FITNESS_TYPE>
class RouletteWheelSelection : public BaseSelectionOperation<FITNESS_TYPE>{
public:
	RouletteWheelSelection(SelectionSettings* settings): BaseSelectionOperation<FITNESS_TYPE>(settings) {};
	virtual ~RouletteWheelSelection(){};

	virtual typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set doSelect(
			const Population::Population<FITNESS_TYPE> &population,
			BaseManager<FITNESS_TYPE> &manager);

};


} /* namespace Selection */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#include <geneial/core/operations/selection/RouletteWheelSelection.hpp>

#endif /* ROULETTEWHEELSELECTION_H_ */
