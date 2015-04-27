#pragma once

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>

namespace geneial
{
namespace algorithm
{
namespace stopping_criteria
{

using namespace geneial::population::management;

template<typename FITNESS_TYPE>
class FitnessValueDeltaCriterion: public BaseStoppingCriterion<FITNESS_TYPE>
{
public:
    FitnessValueDeltaCriterion(FITNESS_TYPE desiredFitness, FITNESS_TYPE delta) :
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
    FITNESS_TYPE _desiredFitness;
    FITNESS_TYPE _delta;
};

} /* namespace stopping_criteria */
} /* namespace algorithm */
} /* namespace geneial */

