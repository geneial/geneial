#pragma once

#include <geneial/core/population/management/BaseManager.h>

namespace geneial
{
namespace algorithm
{
namespace stopping_criteria
{

using namespace geneial::population::management;

/**
 * @brief checks whether the desired fitness was reached
 */
template<typename FITNESS_TYPE>
class FitnessValueReachedCriterion: public BaseStoppingCriterion<FITNESS_TYPE>
{
private:
    FITNESS_TYPE _desiredFitness;

public:
    FitnessValueReachedCriterion(FITNESS_TYPE desiredFitness) :
            _desiredFitness(desiredFitness)
    {
    }

    virtual ~FitnessValueReachedCriterion()
    {
    }

    virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager)
    {
        return !(manager.getHighestFitness() == _desiredFitness);
    }

    virtual void print(std::ostream& os) const
    {
        os << "FitnessValue (" << _desiredFitness << ")";
    }

};

} /* namespace stopping_criteria */
} /* namespace algorithm */
} /* namespace geneial */

