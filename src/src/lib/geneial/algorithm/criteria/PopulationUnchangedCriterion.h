/*
 * BaseStopCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef BASESTOPPINGCRITERION_H_
#define BASESTOPPINGCRITERION_H_

#include <geneial/core/population/management/BaseManager.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

//TODO (bewo) Idea to implement here:
//Check the oldest age of the youngest chromosome against the previous run and if it has not changed abort

template <typename FITNESS_TYPE>
class FitnessValueReachedCriterion{
public:
	virtual ~FitnessValueReachedCriterion(){};
	virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager) = 0;
};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* BASESTOPPINGCRITERION_H_ */
