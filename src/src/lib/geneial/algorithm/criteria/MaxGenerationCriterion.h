#pragma once

#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>
#include <geneial/core/population/Population.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
geneial_private_namespace(stopping_criteria)
{
using ::geneial::population::management::BaseManager;
using ::geneial::population::Population;

geneial_export_namespace
{

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

} /* geneial_export_namespace */
} /* private namespace stopping_criteria */
} /* private namespace algorithm */
} /* private namespace geneial */

