#pragma once

#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>

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

} /* geneial_export_namespace */
} /* private namespace stopping_criteria */
} /* private namespace algorithm */
} /* private namespace geneial */

