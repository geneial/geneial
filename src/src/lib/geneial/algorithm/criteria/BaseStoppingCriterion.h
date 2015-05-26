#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/management/BaseManager.h>


geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
geneial_private_namespace(stopping_criteria)
{
using ::geneial::utility::Printable;
using ::geneial::population::management::BaseManager;

geneial_export_namespace
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

} /* geneial_export_namespace */
} /* private namespace stopping_criteria */
} /* private namespace algorithm */
} /* private namespace geneial */
