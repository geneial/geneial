#pragma once

#include <geneial/namespaces.h>
#include <geneial/algorithm/BaseGeneticAlgorithm.h>
#include <geneial/core/population/management/BaseManager.h>

#include <set>


geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
using ::geneial::algorithm::stopping_criteria::BaseStoppingCriterion;
using ::geneial::population::management::BaseManager;

geneial_export_namespace
{
template<typename FITNESS_TYPE>
class AlgorithmObserver
{
public:
    enum ObserveableEvent
    {
        GENERATION_DONE, CRITERIA_REACHED
    };

    virtual ~AlgorithmObserver()
    {
    }

    virtual void updateGeneration(BaseManager<FITNESS_TYPE> &manager)
    {
    }

    virtual void updateCriteriaReached(BaseManager<FITNESS_TYPE> &manager,
            const BaseStoppingCriterion<FITNESS_TYPE>& criteria)
    {
    }

    const std::set<ObserveableEvent>& getSubscribedEvents() const
    {
        return _subscribedEvents;
    }

    void setSubscribedEvents(const std::set<ObserveableEvent>& subscribedEvents)
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

} /* geneial_export_namespace */
} /* private namespace algorithm */
} /* private namespace geneial */

