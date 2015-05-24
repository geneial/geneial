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

using namespace geneial::population::management;

template<typename FITNESS_TYPE>
class FitnessValueDeltaCriterion: public BaseStoppingCriterion<FITNESS_TYPE>
{
public:
    FitnessValueDeltaCriterion(const FITNESS_TYPE desiredFitness, const FITNESS_TYPE delta) :
            _desiredFitness(desiredFitness), _delta(delta)
    {
        assert(_delta <= _desiredFitness);
    }

    virtual ~FitnessValueDeltaCriterion()
    {
    }

    virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager)
    {
        //TODO (bewo) git rid of infty ) (true delta)
        return !(manager.getHighestFitness() >= _desiredFitness - _delta);
    }

    virtual void print(std::ostream& os) const
    {
        os << "FitnessValueDelta (" << _desiredFitness << ")";
    }

private:
    const FITNESS_TYPE _desiredFitness;
    const FITNESS_TYPE _delta;
};

} /* namespace exports */
} /* namespace __stopping_criteria_impl */
} /* namespace __algorithm_impl */
} /* namespace __geneial_noexport */
