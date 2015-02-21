/*
 * BaseStopCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef STATEFUL_STOPPING_CRITERION_H_
#define STATEFUL_STOPPING_CRITERION_H_

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

//Encapsulates when a StoppingCriterion has a state and makes sure wasReached has no sideeffects
//and ensures wasStatefully reached is only called once per generation
template <typename FITNESS_TYPE>
class StatefulStoppingCriterion: public BaseStoppingCriterion<FITNESS_TYPE>
{
public:
	StatefulStoppingCriterion():_lastGenerationChecked(0),_cachedResult(false),_wasInvoked(false){}

	virtual ~StatefulStoppingCriterion(){};
	
	virtual bool wasStatefullyReached(BaseManager<FITNESS_TYPE> &manager) = 0;

	virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager){
		const unsigned int currentAge = manager.getPopulation().getAge();

		if( (!_wasInvoked) || (_lastGenerationChecked != currentAge) ){
			assert((wasInvoked)?(_lastGenerationChecked < currentAge):(_lastGenerationChecked <= currentAge));
			_cachedResult = wasStatefullyReached(manager);
			_lastGenerationChecked = manager.getPopulation().getAge();
		}

		_wasInvoked = true;
		return _cachedResult;
	};

private:
	unsigned int _lastGenerationChecked;
	bool _cachedResult;
	bool _wasInvoked;
};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* STATEFUL_STOPPING_CRITERION_H_ */
