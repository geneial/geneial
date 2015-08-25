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

} /* geneial_export_namespace */
} /* private namespace stopping_criteria */
} /* private namespace algorithm */
} /* private namespace geneial */
