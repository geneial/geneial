#pragma once

#include <geneial/algorithm/BaseGeneticAlgorithm.h>
#include <geneial/core/population/management/BaseManager.h>

#include <set>


namespace __geneial_noexport
{
namespace __algorithm_impl
{
using ::geneial::algorithm::stopping_criteria::BaseStoppingCriterion;
using ::geneial::population::management::BaseManager;

inline namespace exports
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

} /* namespace __geneial_noexport */
} /* namespace __algorithm_impl */
} /* namespace exports */

namespace geneial
{
namespace algorithm
{
    using namespace ::__geneial_noexport::__algorithm_impl::exports;
}
}

