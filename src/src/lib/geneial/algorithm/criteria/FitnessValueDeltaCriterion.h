#pragma once

#include <cmath>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>
#include <stdexcept>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(algorithm)
    {
        geneial_private_namespace(stopping_criteria)
        {
            using ::geneial::population::management::BaseManager;

            geneial_export_namespace
            {

                template <typename FITNESS_TYPE>
                class FitnessValueDeltaCriterion : public BaseStoppingCriterion<FITNESS_TYPE>
                {
                  public:
                    FitnessValueDeltaCriterion(const FITNESS_TYPE desiredFitness, const FITNESS_TYPE delta)
                        : _desiredFitness(desiredFitness), _delta(delta)
                    {
                        if (_delta < 0)
                        {
                            throw std::invalid_argument("Fitness tolerance cannot be negative");
                        }
                    }

                    virtual ~FitnessValueDeltaCriterion() {}

                    virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager)
                    {
                        return std::abs(static_cast<long double>(manager.getHighestFitness()) - _desiredFitness) <=
                               _delta;
                    }

                    virtual void print(std::ostream &os) const
                    {
                        os << "FitnessValueDelta (" << _desiredFitness << ")";
                    }

                  private:
                    const FITNESS_TYPE _desiredFitness;
                    const FITNESS_TYPE _delta;
                };

            } /* geneial_export_namespace */
        } /* private namespace stopping_criteria */
    } /* private namespace algorithm */
} /* private namespace geneial */
