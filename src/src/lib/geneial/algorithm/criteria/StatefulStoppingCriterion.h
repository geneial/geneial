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
                /** Caches one observation per manager/generation. Reset explicitly after same-age restarts. */
                template <typename FITNESS_TYPE>
                class StatefulStoppingCriterion : public BaseStoppingCriterion<FITNESS_TYPE>
                {
                  public:
                    virtual void reset()
                    {
                        _lastManager.reset();
                        _wasInvoked = false;
                        _cachedResult = false;
                        _lastGenerationChecked = 0;
                    }

                    virtual bool wasStatefullyReached(BaseManager<FITNESS_TYPE> &manager) = 0;

                    bool wasReached(BaseManager<FITNESS_TYPE> &manager) override
                    {
                        const auto currentAge = manager.getPopulation().getAge();
                        if (_wasInvoked &&
                            (_lastManager.lock().get() != &manager || currentAge < _lastGenerationChecked))
                            reset();
                        if (!_wasInvoked || _lastGenerationChecked != currentAge)
                        {
                            _cachedResult = wasStatefullyReached(manager);
                            _lastGenerationChecked = currentAge;
                            _lastManager = manager.weak_from_this();
                            _wasInvoked = true;
                        }
                        return _cachedResult;
                    }

                  private:
                    std::weak_ptr<BaseManager<FITNESS_TYPE>> _lastManager;
                    unsigned int _lastGenerationChecked = 0;
                    bool _cachedResult = false;
                    bool _wasInvoked = false;
                };
            }
        }
    }
}
