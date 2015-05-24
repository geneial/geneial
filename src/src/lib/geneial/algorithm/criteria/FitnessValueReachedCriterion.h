#pragma once

#include <geneial/core/population/management/BaseManager.h>

namespace __geneial_noexport
{
namespace __algorithm_impl
{
namespace __stopping_criteria_impl
{
using ::geneial::utility::Printable;
using ::geneial::population::management::BaseManager;

inline namespace exports
{
/**
 * @brief checks whether the desired fitness was reached
 */
template<typename FITNESS_TYPE>
class FitnessValueReachedCriterion: public BaseStoppingCriterion<FITNESS_TYPE>
{
private:
    const FITNESS_TYPE _desiredFitness;

public:
    explicit FitnessValueReachedCriterion(const FITNESS_TYPE desiredFitness) :
            _desiredFitness(desiredFitness)
    {
    }

    virtual ~FitnessValueReachedCriterion()
    {
    }

    virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager)
    {
        return (manager.getHighestFitness() >= _desiredFitness);
    }

    virtual void print(std::ostream& os) const
    {
        os << "FitnessValue (" << _desiredFitness << ")";
    }

};

} /* namespace exports */
} /* namespace __stopping_criteria_impl */
} /* namespace __algorithm_impl */
} /* namespace __geneial_noexport */

