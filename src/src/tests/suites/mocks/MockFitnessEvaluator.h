/*
 * MockFitnessEvaluator.h
 *
 *  Created on: Feb 17, 2015
 *      Author: bewo
 */

#ifndef MOCKFITNESSEVALUATOR_H_
#define MOCKFITNESSEVALUATOR_H_

#include "boost/shared_ptr.hpp"
#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/fitness/FitnessEvaluator.h>

using namespace GeneticLibrary;

class MockDoubleFitnessEvaluator: public FitnessEvaluator<double>
{
public:
	typedef boost::shared_ptr<MockDoubleFitnessEvaluator> ptr;
	typedef boost::shared_ptr<const MockDoubleFitnessEvaluator> const_ptr;
	MockDoubleFitnessEvaluator():FitnessEvaluator() {}
	Fitness<double>::ptr evaluate(const BaseChromosome<double>::ptr chromosome) const{
		boost::shared_ptr<Fitness<double> > ptr(new Fitness<double>(1));
		return ptr;
	}
	virtual ~MockDoubleFitnessEvaluator(){};
};

class MockIntFitnessEvaluator: public FitnessEvaluator<int>
{
public:
	typedef boost::shared_ptr<MockIntFitnessEvaluator> ptr;
	typedef boost::shared_ptr<const MockIntFitnessEvaluator> const_ptr;
	MockIntFitnessEvaluator(){};
	Fitness<int>::ptr evaluate(const BaseChromosome<int>::ptr chromosome) const{
		boost::shared_ptr<Fitness<int> > ptr(new Fitness<int>(1));
		return ptr;
	}
	virtual ~MockIntFitnessEvaluator(){};
};


#endif /* MOCKFITNESSEVALUATOR_H_ */
