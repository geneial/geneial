#pragma once

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>
#include <geneial/core/population/Population.h>

namespace __geneial_noexport
{
namespace __algorithm_impl
{
namespace __stopping_criteria_impl
{
using ::geneial::population::management::BaseManager;
using ::geneial::population::Population;

inline namespace exports
{
using namespace geneial::algorithm::stopping_criteria;

template<typename FITNESS_TYPE>
class MaxGenerationCriterion: public BaseStoppingCriterion<FITNESS_TYPE>
{
private:
    const typename Population<FITNESS_TYPE>::population_age _max;

public:
    explicit MaxGenerationCriterion(const long num_max_iterations) :
            _max(num_max_iterations)
    {
    }

    virtual ~MaxGenerationCriterion()
    {
    }

    virtual inline bool wasReached(BaseManager<FITNESS_TYPE> &manager)
    {
        return !(manager.getPopulation().getAge() < _max);
    }

    virtual void print(std::ostream& os) const
    {
        os << "MaxGenerationCriterion (" << _max << ")";
    }

};

} /* namespace exports */
} /* namespace __stopping_criteria_impl */
} /* namespace __algorithm_impl */
} /* namespace __geneial_noexport */

