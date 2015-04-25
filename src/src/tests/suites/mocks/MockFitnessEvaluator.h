#ifndef __GENEIAL_TEST_MOCK_FITNESS_EVALUATOR_H_
#define __GENEIAL_TEST_MOCK_FITNESS_EVALUATOR_H_

#include <boost/shared_ptr.hpp>
#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/fitness/FitnessEvaluator.h>

namespace test_mock
{

using namespace geneial;

class MockDoubleFitnessEvaluator: public FitnessEvaluator<double>
{
public:
    typedef boost::shared_ptr<MockDoubleFitnessEvaluator> ptr;
    typedef boost::shared_ptr<const MockDoubleFitnessEvaluator> const_ptr;
    MockDoubleFitnessEvaluator() :
            FitnessEvaluator()
    {
    }
    Fitness<double>::ptr evaluate(const BaseChromosome<double>::ptr chromosome) const
    {
        boost::shared_ptr<Fitness<double> > ptr(new Fitness<double>(1));
        return ptr;
    }
    virtual ~MockDoubleFitnessEvaluator()
    {
    }
};

class MockIntFitnessEvaluator: public FitnessEvaluator<int>
{
public:
    typedef boost::shared_ptr<MockIntFitnessEvaluator> ptr;
    typedef boost::shared_ptr<const MockIntFitnessEvaluator> const_ptr;
    MockIntFitnessEvaluator()
    {
    }
    Fitness<int>::ptr evaluate(const BaseChromosome<int>::ptr chromosome) const
    {
        boost::shared_ptr<Fitness<int> > ptr(new Fitness<int>(1));
        return ptr;
    }
    virtual ~MockIntFitnessEvaluator()
    {
    }
};

} /* namespace test_mock */

#endif /* __GENEIAL_TEST_MOCK_FITNESS_EVALUATOR_H_ */
