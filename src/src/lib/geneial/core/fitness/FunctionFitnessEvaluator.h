#pragma once

#include <geneial/core/fitness/FitnessEvaluator.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>

#include <functional>
#include <span>
#include <stdexcept>
#include <utility>

namespace geneial
{

/** Wraps a read-only genome callable. Concurrent evaluation requires a reentrant callable. */
template <typename Value, typename Score = double> class FunctionFitnessEvaluator final : public FitnessEvaluator<Score>
{
  private:
    std::function<Score(std::span<const Value>)> _function;

  public:
    explicit FunctionFitnessEvaluator(std::function<Score(std::span<const Value>)> function)
        : _function(std::move(function))
    {
        if (!_function)
        {
            throw std::invalid_argument("Fitness function cannot be empty");
        }
    }

    std::unique_ptr<Fitness<Score>>
    evaluate(const population::chromosome::BaseChromosome<Score> &chromosome) const override
    {
        const auto &typed =
            dynamic_cast<const population::chromosome::MultiValueChromosome<Value, Score> &>(chromosome);
        return std::make_unique<Fitness<Score>>(_function(typed.getContainer()));
    }
};

} // namespace geneial
