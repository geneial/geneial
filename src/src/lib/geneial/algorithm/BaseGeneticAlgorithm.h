#pragma once

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

#include <memory>
#include <map>
#include <list>

namespace geneial
{
namespace algorithm
{

using namespace geneial::operation;
using namespace geneial::population;
using namespace geneial::population::chromosome;
using namespace geneial::population::management;

template<typename FITNESS_TYPE>
class BaseGeneticAlgorithm
{
protected:
    typedef typename std::map<typename AlgorithmObserver<FITNESS_TYPE>::ObserveableEvent,
            std::vector<std::shared_ptr<AlgorithmObserver<FITNESS_TYPE>>>> observers_map;

    observers_map _observers;

    BaseManager<FITNESS_TYPE> _manager;

    bool _wasSolved;
    bool _wasStarted;

    stopping_criteria::BaseStoppingCriterion<FITNESS_TYPE> & _stoppingCriterion;

    selection::BaseSelectionOperation<FITNESS_TYPE> & _selectionOperation;

    coupling::BaseCouplingOperation<FITNESS_TYPE> &_couplingOperation;

    crossover::BaseCrossoverOperation<FITNESS_TYPE> &_crossoverOperation;

    replacement::BaseReplacementOperation<FITNESS_TYPE> &_replacementOperation;

    mutation::BaseMutationOperation<FITNESS_TYPE> &_mutationOperation;

public:

    BaseGeneticAlgorithm(
            stopping_criteria::BaseStoppingCriterion<FITNESS_TYPE> &stoppingCriterion,
            selection::BaseSelectionOperation<FITNESS_TYPE> &selectionOperation,
            coupling::BaseCouplingOperation<FITNESS_TYPE> &couplingOperation,
            crossover::BaseCrossoverOperation<FITNESS_TYPE> &crossoverOperation,
            replacement::BaseReplacementOperation<FITNESS_TYPE> &replacementOperation,
            mutation::BaseMutationOperation<FITNESS_TYPE> &mutationOperation,
            BaseChromosomeFactory<FITNESS_TYPE> &chromosomeFactory):

            _manager(chromosomeFactory),
            _wasSolved(false),
            _wasStarted(false),
            _stoppingCriterion(stoppingCriterion),
            _selectionOperation(selectionOperation),
            _couplingOperation(couplingOperation),
            _crossoverOperation(crossoverOperation),
            _replacementOperation(replacementOperation),
            _mutationOperation(mutationOperation)
    {
    }

    virtual ~BaseGeneticAlgorithm()
    {
    }

    virtual void solve() = 0;

    virtual void setInitialPopulation(typename Population<FITNESS_TYPE>::chromosome_container &container)
    {
        assert(!_wasStarted);
        _manager.getPopulation().replacePopulation(container);
    }

    //Delegates to manager,
    //*caution*: the user should never directly interact with the manager, rather use this class as facade
    inline typename BaseChromosome<FITNESS_TYPE>::ptr getHighestFitnessChromosome() const
    {
        assert(_wasStarted);
        return _manager.getHighestFitnessChromosome();
    }

    inline FITNESS_TYPE getHighestFitness() const
    {
        assert(_wasStarted);
        return _manager.getHighestFitness();
    }

    virtual Population<FITNESS_TYPE>& getPopulation()
    {
        return _manager.getPopulation();
    }

    inline typename BaseChromosome<FITNESS_TYPE>::ptr getLowestFitnessChromosome() const
    {
        assert(_wasStarted);
        return _manager.getLowestFitnessChromosome();
    }

    inline FITNESS_TYPE getLowestFitness() const
    {
        assert(_wasStarted);
        return _manager.getLowestFitness();
    }

    inline virtual bool hasBeenSolved() const
    {
        return _wasSolved;
    }

    inline virtual bool hasBeenStarted() const
    {
        return _wasStarted;
    }

    inline virtual PopulationSettings& getPopulationSettings()
    {
        return _manager.getPopulationSettings();
    }

    inline void setPopulationSettings(PopulationSettings& populationSettings)
    {
        _manager.setPopulationSettings(populationSettings);
    }

    inline virtual bool wasCriteriaReached();

    inline virtual void notifyObservers(typename AlgorithmObserver<FITNESS_TYPE>::ObserveableEvent event);

    inline virtual void registerObserver(std::shared_ptr<AlgorithmObserver<FITNESS_TYPE>> observer);
};

} /* namespace algorithm */
} /* namespace geneial */

#include <geneial/algorithm/BaseGeneticAlgorithm.hpp>

