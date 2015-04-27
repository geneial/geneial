#pragma once

#include <geneial/core/population/management/BaseManager.h>

namespace geneial
{
namespace algorithm
{
namespace stopping_criteria
{

using namespace geneial::utility;
using namespace geneial::population::management;

template<typename FITNESS_TYPE>
class BaseStoppingCriterion: public Printable
{
public:

    virtual ~BaseStoppingCriterion()
    {
    }
    ;

    virtual void print(std::ostream& os) const = 0;

    //Should return true, if criterion was met. Can use manager to compute population information.
    //NOTE(bewo): This function must be free of any side-effects and might be called multipe times for the same generation!
    virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager) = 0;
};

} /* namespace stopping_criteria */
} /* namespace algorithm */
} /* namespace geneial */

