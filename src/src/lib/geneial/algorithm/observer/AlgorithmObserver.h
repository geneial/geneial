#ifndef __GENEIAL_ALGORITHM_OBSERVER_H_
#define __GENEIAL_ALGORITHM_OBSERVER_H_

#include <geneial/algorithm/BaseGeneticAlgorithm.h>
#include <geneial/core/population/management/BaseManager.h>

#include <set>

namespace geneial {
namespace algorithm {

using namespace geneial::population::management;

template <typename FITNESS_TYPE>
class AlgorithmObserver {
public:
	enum ObserveableEvent{
		GENERATION_DONE,
		CRITERIA_REACHED
	};

	virtual ~AlgorithmObserver(){};


	virtual void updateGeneration(BaseManager<FITNESS_TYPE> &manager){}
	virtual void updateCriteriaReached(BaseManager<FITNESS_TYPE> &manager){}

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
		_subscribedEvents.remove(e);
	}

private:
	std::set<ObserveableEvent> _subscribedEvents;

};

} /* namespace algorithm */
} /* namespace geneial */

#endif /* __GENEIAL_ALGORITHM_OBSERVER_H_ */
