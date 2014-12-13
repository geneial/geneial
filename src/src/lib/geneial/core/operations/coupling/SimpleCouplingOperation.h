/*
 * SimpleCoupling.h
 *
 *  Created on: Dec 12, 2014
 *      Author: bewo
 */

#ifndef SIMPLECOUPLING_H_
#define SIMPLECOUPLING_H_

#include <geneial/core/operations/coupling/BaseCouplingOperation.h>

namespace GeneticLibrary {
namespace Operation {
namespace Coupling {

using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Operation::Selection;
using namespace GeneticLibrary::Operation::Crossover;


/**
 * SimpleCouplingOperation iterates over the mating pool and takes the first two partners, crosses them, and produces the given offspring.
 * A wrap around is in the mating pool is performed, if the coupling settings requires more offspring than required.
 */
template <typename FITNESS_TYPE>
class SimpleCouplingOperation : public BaseCouplingOperation<FITNESS_TYPE> {
public:
	SimpleCouplingOperation(CouplingSettings *settings): BaseCouplingOperation<FITNESS_TYPE>(settings) {};
	virtual ~SimpleCouplingOperation() {};

	virtual typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set doCopulate
				(
						typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &mating_pool,
						BaseCrossoverOperation<FITNESS_TYPE> *crossoverOperation,
						BaseManager<FITNESS_TYPE> &manager
				);

};

} /* namespace Coupling */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#include <geneial/core/operations/coupling/SimpleCouplingOperation.hpp>

#endif /* SIMPLECOUPLING_H_ */
