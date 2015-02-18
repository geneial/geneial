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
/**
  * @brief checks whether the desired fitness was reached
*/
template <typename FITNESS_TYPE>
class FitnessValueReachedCriterion : public BaseStoppingCriterion<FITNESS_TYPE> {
private:
	FITNESS_TYPE _desiredFitness;

public:
	FitnessValueReachedCriterion(FITNESS_TYPE desiredFitness):_desiredFitness(desiredFitness){};
	virtual ~FitnessValueReachedCriterion() {};
	virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager){
		return !(manager.getPopulation().getHighestFitness() == _desiredFitness);
	}
};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* BASESTOPPINGCRITERION_H_ */
