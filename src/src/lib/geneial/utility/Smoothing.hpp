#pragma once

#include <geneial/utility/Smoothing.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <type_traits>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(utility)
    {
        using ::geneial::population::chromosome::MultiValueChromosome;
        geneial_export_namespace
        {
            template <typename VALUE_TYPE, typename FITNESS_TYPE>
            void Smoothing::restoreSmoothness(typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr chromosome,
                                              VALUE_TYPE maxAbsElevation, VALUE_TYPE min, VALUE_TYPE max, bool hasStart,
                                              VALUE_TYPE start)
            {
                const auto finite = [](VALUE_TYPE value) { return std::isfinite(static_cast<long double>(value)); };
                if (!chromosome || !finite(min) || !finite(max) || !finite(maxAbsElevation) || min > max ||
                    maxAbsElevation < 0 || (hasStart && (!finite(start) || start < min || start > max)))
                    throw std::invalid_argument("Invalid smoothing chromosome, limits, elevation, or starting anchor");
                auto &values = chromosome->getContainer();
                if (!std::all_of(values.begin(), values.end(), finite))
                    throw std::invalid_argument("Smoothing requires finite gene values");
                if (values.empty())
                    return;
                auto previous = hasStart ? start : std::clamp(values.front(), min, max);
                for (auto &value : values)
                {
                    VALUE_TYPE lower, upper;
                    if constexpr (std::is_integral_v<VALUE_TYPE>)
                    {
                        using Unsigned = std::make_unsigned_t<
                            std::conditional_t<std::is_same_v<VALUE_TYPE, bool>, unsigned int, VALUE_TYPE>>;
                        const auto lowerDistance =
                            static_cast<Unsigned>(static_cast<Unsigned>(previous) - static_cast<Unsigned>(min));
                        const auto upperDistance =
                            static_cast<Unsigned>(static_cast<Unsigned>(max) - static_cast<Unsigned>(previous));
                        lower = lowerDistance < static_cast<Unsigned>(maxAbsElevation)
                                    ? min
                                    : static_cast<VALUE_TYPE>(previous - maxAbsElevation);
                        upper = upperDistance < static_cast<Unsigned>(maxAbsElevation)
                                    ? max
                                    : static_cast<VALUE_TYPE>(previous + maxAbsElevation);
                    }
                    else
                    {
                        lower = std::max(min, previous - maxAbsElevation);
                        upper = std::min(max, previous + maxAbsElevation);
                    }
                    value = std::clamp(value, lower, upper);
                    previous = value;
                }
            }

            template <typename VALUE_TYPE, typename FITNESS_TYPE>
            void Smoothing::peakAt(unsigned int pos, unsigned int epsLeft, unsigned int epsRight, VALUE_TYPE elevation,
                                   typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr chromosome)
            {
                if (!chromosome || pos >= chromosome->getSize() || !std::isfinite(static_cast<long double>(elevation)))
                    throw std::invalid_argument("A peak needs a valid chromosome position and finite elevation");
                auto &values = chromosome->getContainer();
                const std::size_t begin = pos - std::min(pos, epsLeft);
                const std::size_t end = pos + std::min<std::size_t>(epsRight, values.size() - pos - 1);
                if (!std::all_of(values.begin() + begin, values.begin() + end + 1,
                                 [](VALUE_TYPE value) { return std::isfinite(static_cast<long double>(value)); }))
                    throw std::invalid_argument("A peak requires finite gene values");
                for (std::size_t index = begin; index <= end; ++index)
                {
                    const std::size_t distance = index > pos ? index - pos : pos - index;
                    const auto width = index > pos ? epsRight : epsLeft;
                    const long double fraction = distance == 0 ? 1 : 1 - static_cast<long double>(distance) / width;
                    const long double elevated = static_cast<long double>(values[index]) + fraction * elevation;
                    if (elevated <= static_cast<long double>(std::numeric_limits<VALUE_TYPE>::lowest()))
                        values[index] = std::numeric_limits<VALUE_TYPE>::lowest();
                    else if (elevated >= static_cast<long double>(std::numeric_limits<VALUE_TYPE>::max()))
                        values[index] = std::numeric_limits<VALUE_TYPE>::max();
                    else
                        values[index] = static_cast<VALUE_TYPE>(elevated);
                }
            }
        }
    }
}
