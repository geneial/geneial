#pragma once

#include <geneial/algorithm/criteria/StatefulStoppingCriterion.h>

#include <cmath>
#include <deque>
#include <limits>
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
                /** Stops when nonnegative improvement over a bounded observation window is within the threshold. */
                template <typename FITNESS_TYPE>
                class FixPointCriterion : public StatefulStoppingCriterion<FITNESS_TYPE>
                {
                  public:
                    FixPointCriterion(const FITNESS_TYPE threshold = 0, const FITNESS_TYPE windowSize = 10,
                                      const FITNESS_TYPE minDiff = 2)
                        : _windowSize(validateCount(windowSize)), _threshold(threshold),
                          _minDiff(validateCount(minDiff))
                    {
                        if (!std::isfinite(static_cast<long double>(threshold)) || threshold < 0 ||
                            _minDiff > _windowSize)
                            throw std::invalid_argument(
                                "Fix-point threshold must be finite/nonnegative and observation counts valid");
                    }

                    void reset() override
                    {
                        StatefulStoppingCriterion<FITNESS_TYPE>::reset();
                        _window.clear();
                    }

                    bool wasStatefullyReached(BaseManager<FITNESS_TYPE> &manager) override
                    {
                        _window.push_front(manager.getHighestFitness());
                        if (_window.size() > _windowSize)
                            _window.pop_back();
                        if (_window.size() < _minDiff || _window.front() < _window.back())
                            return false;
                        if constexpr (std::is_integral_v<FITNESS_TYPE> && !std::is_same_v<FITNESS_TYPE, bool>)
                        {
                            using Unsigned = std::make_unsigned_t<FITNESS_TYPE>;
                            const auto improvement = static_cast<Unsigned>(static_cast<Unsigned>(_window.front()) -
                                                                           static_cast<Unsigned>(_window.back()));
                            return improvement <= static_cast<Unsigned>(_threshold);
                        }
                        return static_cast<long double>(_window.front()) - static_cast<long double>(_window.back()) <=
                               _threshold;
                    }

                    void print(std::ostream &stream) const override
                    {
                        stream << "FixPoint(threshold:" << _threshold << ",minimum:" << _minDiff
                               << ",window:" << _windowSize << ")";
                    }

                  private:
                    static unsigned int validateCount(FITNESS_TYPE value)
                    {
                        const long double count = value;
                        if (!std::isfinite(count) || count < 2 || count > std::numeric_limits<unsigned int>::max() ||
                            std::floor(count) != count)
                            throw std::invalid_argument(
                                "Fix-point observation counts must be whole numbers of at least two");
                        return static_cast<unsigned int>(value);
                    }

                    const unsigned int _windowSize;
                    const FITNESS_TYPE _threshold;
                    const unsigned int _minDiff;
                    std::deque<FITNESS_TYPE> _window;
                };
            }
        }
    }
}
