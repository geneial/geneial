/*
 * BaseGeneticAlgorithm.h
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef BASEGENETICALGORITHM_H_
#define BASEGENETICALGORITHM_H_

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/core/population/PopulationSettings.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>
#include <geneial/core/operations/selection/BaseSelectionOperation.h>

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Operation;

namespace GeneticLibrary {
namespace Algorithm {

template <typename FITNESS_TYPE>
class BaseGeneticAlgorithm {
private:
	BaseManager<FITNESS_TYPE> _manager;
	StoppingCriteria::BaseStoppingCriterion<FITNESS_TYPE> * _stoppingCriterion;
	Selection::BaseSelectionOperation<FITNESS_TYPE> * _selectionOperation;
public:
	BaseGeneticAlgorithm(
			PopulationSettings *populationSettings,
			BaseChromosomeFactory<FITNESS_TYPE> *chromosomeFactory,
			StoppingCriteria::BaseStoppingCriterion<FITNESS_TYPE> *stoppingCriterion,
			FitnessEvaluator<FITNESS_TYPE> *fitnessEvaluator,
			Selection::BaseSelectionOperation<FITNESS_TYPE> *selectionOperation
	):
		_manager(
			populationSettings,
			chromosomeFactory,
			fitnessEvaluator
			),
		_stoppingCriterion(stoppingCriterion),
		_selectionOperation(selectionOperation)
		{};

	virtual ~BaseGeneticAlgorithm() {};

	void solve();
};

} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#include <geneial/algorithm/BaseGeneticAlgorithm.hpp>

#endif /* BASEGENETICALGORITHM_H_ */
