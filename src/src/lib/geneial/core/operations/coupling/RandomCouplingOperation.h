/*
 * RandomCouplingOperation.h
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
 * RandomCouplingOperation will choose parents from the mating pool at random
 */
template <typename FITNESS_TYPE>
class RandomCouplingOperation : public BaseCouplingOperation<FITNESS_TYPE> {
public:
	RandomCouplingOperation(CouplingSettings *settings): BaseCouplingOperation<FITNESS_TYPE>(settings) {};
	virtual ~RandomCouplingOperation() {};

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

#include <geneial/core/operations/coupling/RandomCouplingOperation.hpp>

#endif /* SIMPLECOUPLING_H_ */
