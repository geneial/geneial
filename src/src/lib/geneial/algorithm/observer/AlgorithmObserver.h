#ifndef ALGORITHMOBSERVER_H_
#define ALGORITHMOBSERVER_H_

#include <geneial/algorithm/BaseGeneticAlgorithm.h>
#include <geneial/core/population/management/BaseManager.h>
#include <set>

namespace GeneticLibrary {
namespace Algorithm {

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Operation;

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

} //Algorithm
} //GeneticLibrary

#endif /* ALGORITHMOBSERVER_H_ */
