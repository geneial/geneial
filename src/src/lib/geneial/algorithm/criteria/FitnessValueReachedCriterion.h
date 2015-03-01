/*
 * BaseStopCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef FITNESS_VALUE_REACHEDCRITERION_H_
#define FITNESS_VALUE_REACHEDCRITERION_H_

#include <geneial/core/population/management/BaseManager.h>


namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

using namespace GeneticLibrary::Population::Manager;

/**
  * @brief checks whether the desired fitness was reached
*/
template <typename FITNESS_TYPE>
class FitnessValueReachedCriterion : public BaseStoppingCriterion<FITNESS_TYPE> {
private:
	FITNESS_TYPE _desiredFitness;

public:
	FitnessValueReachedCriterion(FITNESS_TYPE desiredFitness):_desiredFitness(desiredFitness){}

	virtual ~FitnessValueReachedCriterion() {}

	virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager)
	{
		return !(manager.getHighestFitness() == _desiredFitness);
	}

	virtual void print(std::ostream& os) const
	{
		os << "FitnessValue (" << _desiredFitness << ")";
	}

};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* FITNESS_VALUE_REACHEDCRITERION_H_ */
