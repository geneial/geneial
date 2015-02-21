/*
 * FitnessDeltaCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef FITNESSDELTACRITERION_H_
#define FITNESSDELTACRITERION_H_

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

template <typename FITNESS_TYPE>
class FitnessValueDeltaCriterion: public BaseStoppingCriterion<FITNESS_TYPE> {
public:
	FitnessValueDeltaCriterion(FITNESS_TYPE desiredFitness,FITNESS_TYPE delta):_desiredFitness(desiredFitness),_delta(delta)
	{
		assert(_delta <= _desiredFitness);
	}

	virtual ~FitnessValueDeltaCriterion() {}

	virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager)
	{
		return !(manager.getPopulation().getHighestFitness() >= _desiredFitness - _delta);
	}

	virtual void print(std::ostream& os) const
	{
		os << "FitnessValueDelta (" << _desiredFitness << ")";
	}

private:
	FITNESS_TYPE _desiredFitness;
	FITNESS_TYPE _delta;
};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* FITNESSDELTACRITERION_H_ */
