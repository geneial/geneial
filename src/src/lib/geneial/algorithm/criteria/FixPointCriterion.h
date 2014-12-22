/*
 * FitnessDeltaCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef FITNESSDELTACRITERION_H_
#define FITNESSDELTACRITERION_H_

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

//TODO (bewo) Idea to implement here:
//count the last best xx chromosomes, each iteration count the derivation in the best chromosome
//abort if derivation is below certain threshold (typically 0)


template <typename FITNESS_TYPE>
class FixPointCriterion : public BaseStoppingCriterion<FITNESS_TYPE> {
public:
	virtual ~FixPointCriterion() {};
	virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager){
		//TODO (bewo) here be dragons!
		return true;
	}
};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* FITNESSDELTACRITERION_H_ */
