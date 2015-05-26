#pragma once

#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
geneial_private_namespace(stopping_criteria)
{
using ::geneial::population::management::BaseManager;
geneial_export_namespace
{

//Encapsulates when a StoppingCriterion has a state and makes sure wasReached has no sideeffects
//and ensures wasStatefully reached is only called once per generation
template<typename FITNESS_TYPE>
class StatefulStoppingCriterion: public BaseStoppingCriterion<FITNESS_TYPE>
{
public:
    StatefulStoppingCriterion() :
            _lastGenerationChecked(0), _cachedResult(false), _wasInvoked(false)
    {
    }

    virtual ~StatefulStoppingCriterion()
    {
    }

    virtual bool wasStatefullyReached(BaseManager<FITNESS_TYPE> &manager) = 0;

    virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager)
    {
        const unsigned int currentAge = manager.getPopulation().getAge();

        if ((!_wasInvoked) || (_lastGenerationChecked != currentAge))
        {
            assert((_wasInvoked) ? (_lastGenerationChecked < currentAge) : (_lastGenerationChecked <= currentAge));
            _cachedResult = wasStatefullyReached(manager);
            _lastGenerationChecked = manager.getPopulation().getAge();
        }

        _wasInvoked = true;

        return _cachedResult;
    }

private:

    unsigned int _lastGenerationChecked;

    bool _cachedResult;

    bool _wasInvoked;
};

} /* geneial_export_namespace */
} /* private namespace stopping_criteria */
} /* private namespace algorithm */
} /* private namespace geneial */


