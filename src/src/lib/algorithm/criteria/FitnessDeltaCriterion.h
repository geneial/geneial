/*
 * FitnessDeltaCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef FITNESSDELTACRITERION_H_
#define FITNESSDELTACRITERION_H_

#include <core/population/management/BaseManager.h>
#include <algorithm/criteria/BaseStoppingCriterion.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

template <typename FITNESS_TYPE>
class FitnessDeltaCriterion : public BaseStoppingCriterion<FITNESS_TYPE> {
public:
	virtual ~FitnessDeltaCriterion() {};
	virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager){
		//TODO (bewo) here be dragons!
		return true;
	}
};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* FITNESSDELTACRITERION_H_ */
