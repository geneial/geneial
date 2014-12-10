/*
 * BaseGeneticAlgorithm.h
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef BASEGENETICALGORITHM_H_
#define BASEGENETICALGORITHM_H_

#include <core/population/management/BaseManager.h>
#include <core/population/PopulationSettings.h>
#include <algorithm/criteria/BaseStoppingCriterion.h>

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Population::Chromosome;

namespace GeneticLibrary {
namespace Algorithm {

template <typename FITNESS_TYPE>
class BaseGeneticAlgorithm {
private:
	BaseManager<FITNESS_TYPE> _manager;
	StoppingCriteria::BaseStoppingCriterion<FITNESS_TYPE> * _stoppingCriterion;
public:
	BaseGeneticAlgorithm(
			PopulationSettings *populationSettings,
			BaseChromosomeFactory<FITNESS_TYPE> *chromosomeFactory,
			StoppingCriteria::BaseStoppingCriterion<FITNESS_TYPE> *stoppingCriterion
	):
		_manager(
			*populationSettings,
			*chromosomeFactory
			),
		_stoppingCriterion(stoppingCriterion){};
	virtual ~BaseGeneticAlgorithm();

	void solve();
};

} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* BASEGENETICALGORITHM_H_ */
