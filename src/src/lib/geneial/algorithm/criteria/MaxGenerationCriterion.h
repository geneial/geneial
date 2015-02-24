/*
 * MaxGenerationCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef MAX_GENERATION_CRITERION_H_
#define MAX_GENERATION_CRITERION_H_

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>
#include <geneial/core/population/Population.h>


namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

using namespace GeneticLibrary::Population::Manager;

template <typename FITNESS_TYPE>
class MaxGenerationCriterion: public BaseStoppingCriterion<FITNESS_TYPE> {
private:
	typename Population<FITNESS_TYPE>::population_age _max;

public:
	MaxGenerationCriterion (long num_max_iterations):_max(num_max_iterations){};

	virtual ~MaxGenerationCriterion () {};

	virtual inline bool wasReached(BaseManager<FITNESS_TYPE> &manager)
	{
		return !(manager.getPopulation().getAge() < _max);
	}

	virtual void print(std::ostream& os) const
	{
		os << "MaxGenerationCriterion (" << _max << ")";
	}

};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* MAX_GENERATION_CRITERION_H_ */