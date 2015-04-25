#ifndef __GENEIAL_ALGORITHM_OBSERVER_H_
#define __GENEIAL_ALGORITHM_OBSERVER_H_

#include <geneial/algorithm/BaseGeneticAlgorithm.h>
#include <geneial/core/population/management/BaseManager.h>

#include <set>

namespace geneial {
namespace algorithm {

using namespace geneial::population::management;
using namespace geneial::algorithm::stopping_criteria;

template <typename FITNESS_TYPE>
class AlgorithmObserver {
public:
	enum ObserveableEvent{
		GENERATION_DONE,
		CRITERIA_REACHED
	};

	virtual ~AlgorithmObserver(){}


	virtual void updateGeneration(BaseManager<FITNESS_TYPE> &manager){}

	virtual void updateCriteriaReached(BaseManager<FITNESS_TYPE> &manager, const BaseStoppingCriterion<FITNESS_TYPE> * const criteria){}

	const std::set<ObserveableEvent>& getSubscribedEvents() const
	{
		return _subscribedEvents;
	}

	void setSubscribedEvents(
			const std::set<ObserveableEvent>& subscribedEvents)
	{
		_subscribedEvents = subscribedEvents;
	}

	void addSubscribedEvent(ObserveableEvent e)
	{
		_subscribedEvents.insert(e);
	}

	void removeSubscribedEvent(ObserveableEvent e)
	{
		_subscribedEvents.erase(e);
	}

private:
	std::set<ObserveableEvent> _subscribedEvents;

};

} /* namespace algorithm */
} /* namespace geneial */

#endif /* __GENEIAL_ALGORITHM_OBSERVER_H_ */
