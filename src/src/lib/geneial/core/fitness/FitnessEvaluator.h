#ifndef __GENEIAL_FITNESS_EVALUATOR_H_
#define __GENEIAL_FITNESS_EVALUATOR_H_

#include <boost/shared_ptr.hpp>
#include <geneial/core/population/chromosome/BaseChromosome.h>

//Forward Declaration for BaseChromosome (due to circular inclusion)
namespace geneial{
namespace population{
namespace chromosome{
	template <typename FITNESS_TYPE>
	class BaseChromosome;
}
}
}


namespace geneial{

using namespace geneial::population::chromosome;

template <typename FITNESS_TYPE>
class FitnessEvaluator {
public:
	typedef boost::shared_ptr<FitnessEvaluator <FITNESS_TYPE> > ptr;

	typedef boost::shared_ptr<const FitnessEvaluator <FITNESS_TYPE> > const_ptr;

	//TODO (CODECONSISTENCY) (bewo): shared from this?

	FitnessEvaluator() {}

	virtual ~FitnessEvaluator(){}

	virtual typename Fitness<FITNESS_TYPE>::ptr evaluate(const typename BaseChromosome<FITNESS_TYPE>::ptr chromosome) const = 0;
};

} /* namespace GeneticLibrary */

#endif /* __GENEIAL_FITNESS_EVALUATOR_H_ */
