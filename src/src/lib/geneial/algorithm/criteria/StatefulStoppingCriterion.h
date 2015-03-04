#ifndef __GENEIAL_STATEFUL_STOPPING_CRITERION_H_
#define __GENEIAL_STATEFUL_STOPPING_CRITERION_H_

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>

namespace geneial {
namespace algorithm {
namespace stopping_criteria {

using namespace geneial::population::management;

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
			assert((_wasInvoked)?(_lastGenerationChecked < currentAge):(_lastGenerationChecked <= currentAge));
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

} /* namespace stopping_criteria */
} /* namespace algorithm */
} /* namespace geneial */

#endif /* __GENEIAL_STATEFUL_STOPPING_CRITERION_H_ */
