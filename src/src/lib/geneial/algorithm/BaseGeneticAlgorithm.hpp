#pragma once

#include <geneial/algorithm/BaseGeneticAlgorithm.h>

#include <iterator>
#include <set>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
geneial_export_namespace
{

template<typename FITNESS_TYPE>
inline bool BaseGeneticAlgorithm<FITNESS_TYPE>::wasCriteriaReached() //TODO(bewo) Rethink about constness of this
{
    const bool wasReached = _stoppingCriterion->wasReached(*_manager);
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

            case AlgorithmObserver<FITNESS_TYPE>::AFTER_GENERATION:
            {
                for (const auto& it : lb->second)
                {
                    it->updateAfterGeneration(*_manager);
                }
                break;
            }

            case AlgorithmObserver<FITNESS_TYPE>::BEFORE_GENERATION:
            {
                for (const auto& it : lb->second)
                {
                    it->updateBeforeGeneration(*_manager);
                }
                break;
            }
            case AlgorithmObserver<FITNESS_TYPE>::CRITERIA_REACHED:
            {
                for (const auto& it : lb->second)
                {
                    it->updateCriteriaReached(*_manager, *_stoppingCriterion);
                }
                break;
            }
        }
    }
}

template<typename FITNESS_TYPE>
inline void  BaseGeneticAlgorithm<FITNESS_TYPE>::registerObserver(const std::shared_ptr<AlgorithmObserver<FITNESS_TYPE>> &observer)
{
    typedef std::set<typename AlgorithmObserver<FITNESS_TYPE>::ObserveableEvent> setType;
    const setType events = observer->getSubscribedEvents();
    for (const auto &it :events)
    {

        typename observers_map::iterator lb = _observers.lower_bound(it);

        if (lb != _observers.end() && !(_observers.key_comp()(it, lb->first)))
        {
            // key already exists
            // update lb->second if you care to
            lb->second.emplace_back(observer);
        }
        else
        {
            // the key does not exist in the map
            // add it to the map
            typename std::vector<std::shared_ptr<AlgorithmObserver<FITNESS_TYPE>>> list;
            list.emplace_back(observer);
            _observers.insert(lb, typename observers_map::value_type(it, list));    // Use lb as a hint to insert,
        }

    }

}

} /* geneial_export_namespace */
} /* private namespace algorithm */
} /* private namespace geneial */
