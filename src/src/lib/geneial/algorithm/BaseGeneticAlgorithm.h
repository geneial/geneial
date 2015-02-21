/*
 * BaseGeneticAlgorithm.h
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef BASEGENETICALGORITHM_H_
#define BASEGENETICALGORITHM_H_

#include <geneial/core/population/Population.h>
#include <geneial/core/population/PopulationSettings.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>
#include <geneial/core/operations/selection/BaseSelectionOperation.h>
#include <geneial/core/operations/coupling/BaseCouplingOperation.h>
#include <geneial/core/operations/crossover/BaseCrossoverOperation.h>
#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/operations/replacement/BaseReplacementOperation.h>
#include <geneial/core/population/management/BaseManager.h>

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
	bool _wasSolved;
	bool _wasStarted;
	StoppingCriteria::BaseStoppingCriterion<FITNESS_TYPE> * _stoppingCriterion;
	Selection::BaseSelectionOperation<FITNESS_TYPE> * _selectionOperation;
	Coupling::BaseCouplingOperation<FITNESS_TYPE> *_couplingOperation;
	Crossover::BaseCrossoverOperation<FITNESS_TYPE> *_crossoverOperation;
	Replacement::BaseReplacementOperation<FITNESS_TYPE> *_replacementOperation;
	Mutation::BaseMutationOperation<FITNESS_TYPE> *_mutationOperation;
public:
	BaseGeneticAlgorithm(
			PopulationSettings *populationSettings,
			BaseChromosomeFactory<FITNESS_TYPE> *chromosomeFactory,

			StoppingCriteria::BaseStoppingCriterion<FITNESS_TYPE> *stoppingCriterion,

			Selection::BaseSelectionOperation<FITNESS_TYPE> *selectionOperation,
			Coupling::BaseCouplingOperation<FITNESS_TYPE> *couplingOperation,
			Crossover::BaseCrossoverOperation<FITNESS_TYPE> *crossoverOperation,
			Replacement::BaseReplacementOperation<FITNESS_TYPE> *replacementOperation,
			Mutation::BaseMutationOperation<FITNESS_TYPE> *mutationOperation

	):
		_manager(
			populationSettings,
			chromosomeFactory
			),
		_wasSolved(false),
		_wasStarted(false),
		_stoppingCriterion(stoppingCriterion),
		_selectionOperation(selectionOperation),
		_couplingOperation(couplingOperation),
		_crossoverOperation(crossoverOperation),
		_replacementOperation(replacementOperation),
		_mutationOperation(mutationOperation)
		{};

	virtual ~BaseGeneticAlgorithm() {};

	virtual void solve();

	virtual void setInitialPopulation(typename Population::Population<FITNESS_TYPE>::chromosome_container &container){
		assert(!_wasStarted);
		_manager.getPopulation().replacePopulation(container);
	}

	//Delegates to manager,
	//*caution*: the user should never directly interact with the manager, rather use this class as facade
	inline typename BaseChromosome<FITNESS_TYPE>::ptr getHighestFitnessChromosome() const{
		assert(_wasStarted);
		return _manager.getHighestFitnessChromosome();
	}

	inline FITNESS_TYPE getHighestFitness()  const{
		assert(_wasStarted);
		return _manager.getHighestFitness();
	}


	virtual typename Population::Population<FITNESS_TYPE>& getPopulation(){
			return _manager.getPopulation();
	}

	inline typename BaseChromosome<FITNESS_TYPE>::ptr getLowestFitnessChromosome() const{
		assert(_wasStarted);
		return _manager.getLowestFitnessChromosome();
	}

	inline FITNESS_TYPE getLowestFitness() const{
		assert(_wasStarted);
		return _manager.getLowestFitness();
	}


	inline virtual bool hasBeenSolved() const{
		return _wasSolved;
	}

	inline virtual bool hasBeenStarted() const{
		return _wasStarted;
	}
};

} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#include <geneial/algorithm/BaseGeneticAlgorithm.hpp>

#endif /* BASEGENETICALGORITHM_H_ */
