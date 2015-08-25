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

} /* geneial_export_namespace */
} /* private namespace stopping_criteria */
} /* private namespace algorithm */
} /* private namespace geneial */
