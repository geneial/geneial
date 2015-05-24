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


//TODO (bewo) Idea to implement here: -> c.f. TrailingStop in Stock market
//Trace the best chromosome, when its gets worser as a given threshold abort.

template<typename FITNESS_TYPE>
class TrailingStopCriterion: public BaseStoppingCriterion<FITNESS_TYPE>
{
public:
    virtual ~TrailingStopCriterion()
    {
    }

    virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager)
    {
        //TODO (bewo): unimplemeted!
        return false;
    }

    virtual void print(std::ostream& os) const
    {
        os << "TrailingStop ()";
    }
};

} /* namespace exports */
} /* namespace __stopping_criteria_impl */
} /* namespace __algorithm_impl */
} /* namespace __geneial_noexport */
