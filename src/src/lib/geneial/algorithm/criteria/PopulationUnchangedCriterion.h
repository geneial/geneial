/*
 * BaseStopCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef BASESTOPPINGCRITERION_H_
#define BASESTOPPINGCRITERION_H_

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/StatefulStoppingCriterion.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

template <typename FITNESS_TYPE>
class PopulationUnchangedCriterion: StatefulStoppingCriterion<FITNESS_TYPE>
{
public:

	PopulationUnchangedCriterion() :
			_wasEverInvoked(false), _lastGenerationOldestAge(0), _lastGenerationYoungestAge(
					0), StatefulStoppingCriterion() {}

	virtual ~PopulationUnchangedCriterion(){};

	virtual bool wasStatefullyReached(BaseManager<FITNESS_TYPE> &manager)
	{
		bool result = false;

		if(!_wasEverInvoked)
		{
			_wasEverInvoked = true;
		}
		else
		{
			//Check the oldest age of the youngest chromosome against the previous run and if it has not changed abort
			result = (_lastGenerationOldestAge == manager.getPopulation().getYoungestChromosome()->getAge());
		}

		_lastGenerationOldestAge = manager.getPopulation().getOldestChromosome()->getAge();
		_lastGenerationYoungestAge = manager.getPopulation().getYoungestChromosome()->getAge();

		return result;
	};

	virtual void print(std::ostream& os) const
	{
		os << "Population Unchanged ()";
	}

private:
	//What was the last generation that was checked?
	unsigned int _wasEverInvoked;

	//Last generation age?
	unsigned int _lastGenerationOldestAge;
	unsigned int _lastGenerationYoungestAge;
};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* BASESTOPPINGCRITERION_H_ */
