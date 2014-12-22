/*
 * UniformRandomSelection.h
 *
 *  Created on: Dec 11, 2014
 *      Author: bewo
 */

#ifndef UNIFORM_RANDOM_SELECTION_H_
#define UNIFORM_RANDOM_SELECTION_H_

#include <geneial/core/operations/selection/UniformRandomSelection.h>

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
class UniformRandomSelection : public BaseSelectionOperation<FITNESS_TYPE>{
public:
	UniformRandomSelection(SelectionSettings* settings): BaseSelectionOperation<FITNESS_TYPE>(settings) {};
	virtual ~UniformRandomSelection(){};

	virtual typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set doSelect(
			const Population::Population<FITNESS_TYPE> &population,
			BaseManager<FITNESS_TYPE> &manager);

};


} /* namespace Selection */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#include <geneial/core/operations/selection/UniformRandomSelection.hpp>

#endif /* UNIFORM_RANDOM_SELECTION_H_ */
