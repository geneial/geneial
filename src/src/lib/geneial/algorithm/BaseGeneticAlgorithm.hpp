#pragma once

#include <geneial/algorithm/BaseGeneticAlgorithm.h>

#include <iterator>
#include <set>

namespace geneial
{
namespace algorithm
{

template<typename FITNESS_TYPE>
inline bool BaseGeneticAlgorithm<FITNESS_TYPE>::wasCriteriaReached() //TODO(bewo) Rethink about constness of this
{
    const bool wasReached = _stoppingCriterion.wasReached(_manager);
    if (wasReached)
    {
        notifyObservers(AlgorithmObserver<FITNESS_TYPE>::CRITERIA_REACHED);
    }
    return wasReached;
}

template<typename FITNESS_TYPE>
inline void BaseGeneticAlgorithm<FITNESS_TYPE>::notifyObservers(typename AlgorithmObserver<FITNESS_TYPE>::ObserveableEvent event)
{
    typename observers_map::const_iterator lb = _observers.lower_bound(event);

    if (lb != _observers.end() && !(_observers.key_comp()(event, lb->first)))
    {
        switch (event)
        {
        case AlgorithmObserver<FITNESS_TYPE>::GENERATION_DONE:
        {
            for (typename observers_map::mapped_type::const_iterator it = lb->second.begin();
                    it != lb->second.end(); ++it)
            {
                (*it)->updateGeneration(_manager);
            }
            break;
        }
        case AlgorithmObserver<FITNESS_TYPE>::CRITERIA_REACHED:
        {
            for (typename observers_map::mapped_type::const_iterator it = lb->second.begin();
                    it != lb->second.end(); ++it)
            {
                (*it)->updateCriteriaReached(_manager, _stoppingCriterion);
            }
            break;
        }
        }
    }
}

template<typename FITNESS_TYPE>
inline void  BaseGeneticAlgorithm<FITNESS_TYPE>::registerObserver(AlgorithmObserver<FITNESS_TYPE>* observer)
{
    typedef std::set<typename AlgorithmObserver<FITNESS_TYPE>::ObserveableEvent> setType;
    const setType events = observer->getSubscribedEvents();
    for (typename setType::const_iterator it = events.begin(); it != events.end(); ++it)
    {

        typename observers_map::iterator lb = _observers.lower_bound(*it);

        if (lb != _observers.end() && !(_observers.key_comp()(*it, lb->first)))
        {
            // key already exists
            // update lb->second if you care to
            lb->second.insert(lb->second.begin(), observer);
        }
        else
        {
            // the key does not exist in the map
            // add it to the map
            typename std::list<AlgorithmObserver<FITNESS_TYPE>*> list;
            list.insert(list.begin(), observer);
            _observers.insert(lb, typename observers_map::value_type(*it, list));    // Use lb as a hint to insert,
        }

    }

}


} /* namespace algorithm */
} /* namespace geneial */

