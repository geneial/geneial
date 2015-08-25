#pragma once

#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/fitness/FitnessEvaluator.h>

#include <memory>

namespace test_mock
{

using namespace ::geneial;

template<typename T>
class MockFitnessEvaluator: public FitnessEvaluator<double>
{
    mutable int _calls;
public:

    constexpr const static T MOCK_VALUE = 1;

    using ptr = std::shared_ptr<MockFitnessEvaluator>;
    using const_ptr = std::shared_ptr<const MockFitnessEvaluator>;


    MockFitnessEvaluator() : FitnessEvaluator(), _calls(0)
    {
    }

    std::unique_ptr<Fitness<T>> evaluate(const geneial::population::chromosome::BaseChromosome<T>& chromosome) const
    {
        _calls++;
        return std::move(std::unique_ptr<Fitness<T>>(new Fitness<T>(MOCK_VALUE)));
    }

    virtual ~MockFitnessEvaluator()
    {
    }

    int getCalls() const
    {
        return _calls;
    }

    void setCalls(int calls = 0)
    {
        _calls = calls;
    }
};
} /* namespace test_mock */

