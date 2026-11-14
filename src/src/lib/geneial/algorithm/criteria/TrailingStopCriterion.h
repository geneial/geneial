#pragma once

#include <geneial/algorithm/criteria/StatefulStoppingCriterion.h>

#include <cmath>
#include <optional>
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
                /** Stops after a positive absolute drawdown from the best observed fitness reaches the tolerance. */
                template <typename FITNESS_TYPE>
                class TrailingStopCriterion : public StatefulStoppingCriterion<FITNESS_TYPE>
                {
                  public:
                    explicit TrailingStopCriterion(FITNESS_TYPE maximumDrawdown = 0) : _maximumDrawdown(maximumDrawdown)
                    {
                        if (!std::isfinite(static_cast<long double>(maximumDrawdown)) || maximumDrawdown < 0)
                            throw std::invalid_argument("Trailing-stop drawdown must be finite and nonnegative");
                    }

                    void resetState() override
                    {
                        StatefulStoppingCriterion<FITNESS_TYPE>::resetState();
                        _best.reset();
                    }

                    bool wasStatefullyReached(BaseManager<FITNESS_TYPE> &manager) override
                    {
                        const auto current = manager.getHighestFitness();
                        if (!_best || current >= *_best)
                        {
                            _best = current;
                            return false;
                        }
                        if constexpr (std::is_integral_v<FITNESS_TYPE> && !std::is_same_v<FITNESS_TYPE, bool>)
                        {
                            using Unsigned = std::make_unsigned_t<FITNESS_TYPE>;
                            const auto drawdown =
                                static_cast<Unsigned>(static_cast<Unsigned>(*_best) - static_cast<Unsigned>(current));
                            return drawdown >= static_cast<Unsigned>(_maximumDrawdown);
                        }
                        return static_cast<long double>(*_best) - static_cast<long double>(current) >= _maximumDrawdown;
                    }

                    void print(std::ostream &stream) const override
                    {
                        stream << "TrailingStop(drawdown:" << _maximumDrawdown << ")";
                    }

                  private:
                    const FITNESS_TYPE _maximumDrawdown;
                    std::optional<FITNESS_TYPE> _best;
                };
            }
        }
    }
}
