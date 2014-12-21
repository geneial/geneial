/*
 * BuilderSettings.h
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#ifndef BUILDERSETTINGS_H_
#define BUILDERSETTINGS_H_

#include <geneial/core/fitness/FitnessEvaluator.h>

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

template <typename FITNESS_TYPE>
class BuilderSettings {
private:
	typename FitnessEvaluator<FITNESS_TYPE>::ptr _fitnessEvaluator;
public:
	BuilderSettings(const typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator):
		_fitnessEvaluator(fitnessEvaluator)
	{};

	virtual ~BuilderSettings() {};


	const typename FitnessEvaluator<FITNESS_TYPE>::ptr& getFitnessEvaluator() const {
		return _fitnessEvaluator;
	};

	void setFitnessEvaluator(
			const typename FitnessEvaluator<FITNESS_TYPE>::ptr& fitnessEvaluator) {
		_fitnessEvaluator = fitnessEvaluator;
	};


};

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* BUILDERSETTINGS_H_ */
