#ifndef __GENEIAL_TRAILING_STOP_CRITERION_H_
#define __GENEIAL_TRAILING_STOP_CRITERION_H_

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>

namespace geneial
{
namespace algorithm
{
namespace stopping_criteria
{

using namespace geneial::population::management;

//TODO (bewo) Idea to implement here: -> c.f. TrailingStop in Stock market
//Trace the best chromosome, when its gets worser as a given threshold abort.

template<typename FITNESS_TYPE>
class TrailingStopCriterion: public BaseStoppingCriterion<FITNESS_TYPE>
{
public:
    virtual ~TrailingStopCriterion()
    {
    }
    ;
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

} /* namespace stopping_criteria */
} /* namespace algorithm */
} /* namespace geneial */

#endif /* __GENEIAL_TRAILING_STOP_CRITERION_H_ */
