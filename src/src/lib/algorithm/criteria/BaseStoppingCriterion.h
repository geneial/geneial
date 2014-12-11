/*
 * BaseStopCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef BASESTOPPINGCRITERION_H_
#define BASESTOPPINGCRITERION_H_

#include <core/population/management/BaseManager.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

template <typename FITNESS_TYPE>
class BaseStoppingCriterion{
public:
	virtual ~BaseStoppingCriterion(){};
	virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager) = 0;
};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* BASESTOPPINGCRITERION_H_ */