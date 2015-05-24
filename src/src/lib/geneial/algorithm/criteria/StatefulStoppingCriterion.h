#pragma once

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>

namespace __geneial_noexport
{
namespace __algorithm_impl
{
namespace __stopping_criteria_impl
{
using ::geneial::population::management::BaseManager;

inline namespace exports
{
using namespace geneial::algorithm::stopping_criteria;

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

} /* namespace exports */
} /* namespace __stopping_criteria_impl */
} /* namespace __algorithm_impl */
} /* namespace __geneial_noexport */

