#pragma once

#include <cmath>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>
#include <stdexcept>
#include <type_traits>

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
                        if (!std::isfinite(static_cast<long double>(_desiredFitness)) ||
                            !std::isfinite(static_cast<long double>(_delta)) || _delta < 0)
                        {
                            throw std::invalid_argument(
                                "Fitness target/tolerance must be finite and tolerance nonnegative");
                        }
                    }

                    ~FitnessValueDeltaCriterion() override = default;

                    bool wasReached(BaseManager<FITNESS_TYPE> &manager) override
                    {
                        const auto current = manager.getHighestFitness();
                        if constexpr (std::is_integral_v<FITNESS_TYPE> && !std::is_same_v<FITNESS_TYPE, bool>)
                        {
                            using Unsigned = std::make_unsigned_t<FITNESS_TYPE>;
                            const auto higher = current >= _desiredFitness ? current : _desiredFitness;
                            const auto lower = current >= _desiredFitness ? _desiredFitness : current;
                            const auto difference =
                                static_cast<Unsigned>(static_cast<Unsigned>(higher) - static_cast<Unsigned>(lower));
                            return difference <= static_cast<Unsigned>(_delta);
                        }
                        return std::abs(static_cast<long double>(current) - _desiredFitness) <= _delta;
                    }

                    void print(std::ostream &os) const override
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
