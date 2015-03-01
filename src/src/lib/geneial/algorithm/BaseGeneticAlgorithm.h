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
#include <geneial/algorithm/observer/AlgorithmObserver.h>
#include <map>
#include <list>

namespace GeneticLibrary {
namespace Algorithm {

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Operation;

template <typename FITNESS_TYPE>
class BaseGeneticAlgorithm {
private:
	typedef typename std::map<typename AlgorithmObserver<FITNESS_TYPE>::ObserveableEvent,
			  std::list<AlgorithmObserver<FITNESS_TYPE>* > > observers_map;
	observers_map _observers;
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
			Mutation::BaseMutationOperation<FITNESS_TYPE> *mutationOperation,
			BaseFitnessProcessingStrategy<FITNESS_TYPE>* fitnessProcessingStrategy
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
		{
		_manager.getPopulation().setProcessingStrategy(fitnessProcessingStrategy);
		};

	virtual ~BaseGeneticAlgorithm() {
	};

	virtual void solve();

	virtual void setInitialPopulation(typename Population<FITNESS_TYPE>::chromosome_container &container){
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


	virtual Population<FITNESS_TYPE>& getPopulation(){
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

	inline virtual bool wasCriteriaReached(){
		return _stoppingCriterion->wasReached(_manager);
	}


	void inline notifyObservers(typename AlgorithmObserver<FITNESS_TYPE>::ObserveableEvent event){
		typename observers_map::iterator lb = _observers.lower_bound(event);

		if(lb != _observers.end() && !(_observers.key_comp()(event, lb->first)))
		{
			switch(event)
			{
				case AlgorithmObserver<FITNESS_TYPE>::GENERATION_DONE:
				{
					for(typename observers_map::mapped_type::const_iterator it=lb->second.begin();it!=lb->second.end();++it)
					{
						(*it)->updateGeneration(_manager);
					}
					break;
				}
				case AlgorithmObserver<FITNESS_TYPE>::CRITERIA_REACHED:
				{
					for(typename observers_map::mapped_type::const_iterator it=lb->second.begin();it!=lb->second.end();++it)
					{
						(*it)->updateCriteriaReached(_manager);
					}
					break;
				}
			}
		}
	}

	void registerObserver(AlgorithmObserver<FITNESS_TYPE>* observer)
	{
		typedef std::set<typename AlgorithmObserver<FITNESS_TYPE>::ObserveableEvent> setType;
		const setType events = observer->getSubscribedEvents();
		for(typename setType::const_iterator it=events.begin();it!=events.end();++it){

			typename observers_map::iterator lb = _observers.lower_bound(*it);

			if(lb != _observers.end() && !(_observers.key_comp()(*it, lb->first)))
			{
			    // key already exists
			    // update lb->second if you care to
				lb->second.insert(lb->second.begin(), observer);
			}
			else
			{
			    // the key does not exist in the map
			    // add it to the map
				typename std::list<AlgorithmObserver<FITNESS_TYPE>*> list;
				list.insert(list.begin(),observer);
				_observers.insert(lb, typename observers_map::value_type(*it, list));    // Use lb as a hint to insert,
			}

		}

	}
};

} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#include <geneial/algorithm/BaseGeneticAlgorithm.hpp>

#endif /* BASEGENETICALGORITHM_H_ */
