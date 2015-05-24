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


template<typename FITNESS_TYPE>
class BaseStoppingCriterion: public Printable, public std::enable_shared_from_this<BaseStoppingCriterion<FITNESS_TYPE> >
{
public:

    using ptr = std::shared_ptr<BaseStoppingCriterion<FITNESS_TYPE>>;
    using const_ptr =std::shared_ptr<const BaseStoppingCriterion<FITNESS_TYPE>>;

    virtual ~BaseStoppingCriterion()
    {
    }

    virtual void print(std::ostream& os) const = 0;

    //Should return true, if criterion was met. Can use manager to compute population information.
    //NOTE(bewo): This function must be free of any side-effects and might be called multiple times for the same generation!
    virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager) = 0;
};

} /* namespace exports */
} /* namespace __stopping_criteria_impl */
} /* namespace __algorithm_impl */
} /* namespace __geneial_noexport */

namespace geneial
{
namespace algorithm
{
namespace stopping_criteria
{
    using namespace ::__geneial_noexport::__algorithm_impl::__stopping_criteria_impl::exports;
}
}
}
