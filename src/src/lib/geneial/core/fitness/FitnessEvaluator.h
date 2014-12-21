/*
 * FitnessEvaluator.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef FITNESSEVALUATOR_H_
#define FITNESSEVALUATOR_H_

#include "boost/shared_ptr.hpp"
#include <geneial/core/population/chromosome/BaseChromosome.h>

//Forward Declaration for BaseChromosome (due to circular inclusion)
namespace GeneticLibrary{
namespace Population{
namespace Chromosome{
	template <typename FITNESS_TYPE>
    class BaseChromosome;
}
}
}

using namespace GeneticLibrary::Population::Chromosome;

namespace GeneticLibrary{

template <typename FITNESS_TYPE>
class FitnessEvaluator {
public:
	typedef boost::shared_ptr<FitnessEvaluator <FITNESS_TYPE> > ptr;
	typedef boost::shared_ptr<const FitnessEvaluator <FITNESS_TYPE> > const_ptr;

	FitnessEvaluator() {};
	virtual ~FitnessEvaluator(){};
	virtual typename Fitness<FITNESS_TYPE>::ptr evaluate(const typename BaseChromosome<FITNESS_TYPE>::ptr chromosome) const= 0;
};

} /* namespace GeneticLibrary */

#endif /* FITNESSEVALUATOR_H_ */
