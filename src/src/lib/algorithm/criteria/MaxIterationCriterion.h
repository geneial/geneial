/*
 * MaxIterationCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef MAX_ITERATION_CRITERION_H_
#define MAX_ITERATION_CRITERION_H_

#include <algorithm/criteria/BaseStoppingCriterion.h>
#include <core/population/Population.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

template <typename FITNESS_TYPE>
class MaxIterationCriterion : public BaseStoppingCriterion<FITNESS_TYPE> {
private:
	typename Population::Population<FITNESS_TYPE>::population_age _max;

public:
	MaxIterationCriterion(long num_max_iterations):_max(num_max_iterations){};
	virtual ~MaxIterationCriterion();
	virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager){
		return (manager.getPopulation().getAge() <_max);
	}
};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* MAX_ITERATION_CRITERION_H_ */
