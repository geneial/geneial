#pragma once

#include <geneial/namespaces.h>
#include <geneial/config.h>

#include <geneial/core/population/Population.h>
#include <geneial/core/population/PopulationSettings.h>

#include <geneial/core/population/management/BaseManager.h>

#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>
#include <geneial/core/operations/selection/BaseSelectionOperation.h>
#include <geneial/core/operations/coupling/BaseCouplingOperation.h>
#include <geneial/core/operations/crossover/BaseCrossoverOperation.h>
#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/operations/replacement/BaseReplacementOperation.h>
#include <geneial/algorithm/observer/AlgorithmObserver.h>
#include <geneial/utility/mixins/Buildable.h>

#include <geneial/utility/ExecutionManager.h>

#include <boost/optional.hpp>

#include <memory>
#include <map>
#include <list>
#include <type_traits>


geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
using ::geneial::algorithm::stopping_criteria::BaseStoppingCriterion;
using ::geneial::operation::selection::BaseSelectionOperation;
using ::geneial::operation::coupling::BaseCouplingOperation;
using ::geneial::operation::crossover::BaseCrossoverOperation;
using ::geneial::operation::replacement::BaseReplacementOperation;
using ::geneial::operation::mutation::BaseMutationOperation;
using ::geneial::utility::BaseExecutionManager;
using ::geneial::population::Population;
using ::geneial::population::PopulationSettings;
using ::geneial::population::chromosome::BaseChromosome;
using ::geneial::population::chromosome::BaseChromosomeFactory;
using ::geneial::population::management::BaseBookkeeper;
using ::geneial::population::management::BaseManager;

using ::geneial::utility::Buildable;

geneial_export_namespace
{


template<typename FITNESS_TYPE>
class BaseGeneticAlgorithm : public virtual Buildable<BaseGeneticAlgorithm<FITNESS_TYPE>>
{
protected:
    typedef typename std::map<typename AlgorithmObserver<FITNESS_TYPE>::ObserveableEvent,
            std::vector<std::shared_ptr<AlgorithmObserver<FITNESS_TYPE>>>> observers_map;

    observers_map _observers;

    std::shared_ptr<BaseManager<FITNESS_TYPE>> _manager;

    bool _wasSolved;

    bool _wasStarted;

    std::shared_ptr<BaseStoppingCriterion<FITNESS_TYPE>> _stoppingCriterion;

    std::shared_ptr<BaseSelectionOperation<FITNESS_TYPE>> _selectionOperation;

    std::shared_ptr<BaseCouplingOperation<FITNESS_TYPE>> _couplingOperation;

    std::shared_ptr<BaseCrossoverOperation<FITNESS_TYPE>> _crossoverOperation;

    std::shared_ptr<BaseReplacementOperation<FITNESS_TYPE>> _replacementOperation;

    std::shared_ptr<BaseMutationOperation<FITNESS_TYPE>> _mutationOperation;

    //The client should use the builder instead of instacing the algorithm herself
    BaseGeneticAlgorithm(
            std::shared_ptr<BaseStoppingCriterion<FITNESS_TYPE>> stoppingCriterion,
            std::shared_ptr<BaseSelectionOperation<FITNESS_TYPE>> selectionOperation,
            std::shared_ptr<BaseCouplingOperation<FITNESS_TYPE>> couplingOperation,
            std::shared_ptr<BaseCrossoverOperation<FITNESS_TYPE>> crossoverOperation,
            std::shared_ptr<BaseReplacementOperation<FITNESS_TYPE>> replacementOperation,
            std::shared_ptr<BaseMutationOperation<FITNESS_TYPE>> mutationOperation,
            std::shared_ptr<BaseChromosomeFactory<FITNESS_TYPE>> chromosomeFactory):

            _manager(BaseManager<FITNESS_TYPE>::create(chromosomeFactory)),
            _wasSolved(false),
            _wasStarted(false),
            _stoppingCriterion(stoppingCriterion),
            _selectionOperation(selectionOperation),
            _couplingOperation(couplingOperation),
            _crossoverOperation(crossoverOperation),
            _replacementOperation(replacementOperation),
            _mutationOperation(mutationOperation)
    {
        static_assert(std::is_arithmetic<FITNESS_TYPE>::value,"FITNESS_TYPE is expected to be an arithmetic data type!");
    }

public:
    // use this class to construct an Algorithm
    class Builder;


    virtual ~BaseGeneticAlgorithm()
    {
    }

    virtual void solve() = 0;

    virtual void setInitialPopulation(typename Population<FITNESS_TYPE>::chromosome_container &container)
    {
        assert(!_wasStarted);
        _manager->getPopulation().replacePopulation(container);
    }

    //Delegates to manager,
    //*caution*: the user should never directly interact with the manager, rather use this class as facade
    inline typename BaseChromosome<FITNESS_TYPE>::ptr getHighestFitnessChromosome() const
    {
        assert(_wasStarted);
        return _manager->getHighestFitnessChromosome();
    }

    inline FITNESS_TYPE getHighestFitness() const
    {
        assert(_wasStarted);
        return _manager->getHighestFitness();
    }

    virtual Population<FITNESS_TYPE>& getPopulation()
    {
        return _manager->getPopulation();
    }

    inline typename BaseChromosome<FITNESS_TYPE>::ptr getLowestFitnessChromosome() const
    {
        assert(_wasStarted);
        return _manager->getLowestFitnessChromosome();
    }

    inline FITNESS_TYPE getLowestFitness() const
    {
        assert(_wasStarted);
        return _manager->getLowestFitness();
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
        return _manager->getPopulationSettings();
    }

    inline void setPopulationSettings(PopulationSettings& populationSettings)
    {
        _manager->setPopulationSettings(populationSettings);
    }

    inline virtual bool wasCriteriaReached();

    inline virtual void notifyObservers(typename AlgorithmObserver<FITNESS_TYPE>::ObserveableEvent event);

    inline virtual void registerObserver(const std::shared_ptr<AlgorithmObserver<FITNESS_TYPE>>& observer);


    BaseExecutionManager& getExecutionManager() const
    {
        return _manager->getExecutionManager();
    }

    void setExecutionManager(std::unique_ptr<BaseExecutionManager>&& executionManager)
    {
        _manager->setExecutionManager(std::move(executionManager));
    }

    void setBookkeeper(const std::shared_ptr<BaseBookkeeper>& bookkeeper)
    {
        _manager->setBookkeeper(bookkeeper);
    }

    const std::shared_ptr<BaseBookkeeper> getBookkeeper() const
    {
        return _manager->getBookkeeper();
    }
};

template<typename FITNESS_TYPE>
class BaseGeneticAlgorithm<FITNESS_TYPE>::Builder : public Buildable<BaseGeneticAlgorithm<FITNESS_TYPE>>::Builder
{
protected:
    boost::optional<std::shared_ptr<BaseStoppingCriterion<FITNESS_TYPE>>> _stoppingCriterion;

    boost::optional<std::shared_ptr<BaseSelectionOperation<FITNESS_TYPE>>> _selectionOperation;

    boost::optional<std::shared_ptr<BaseCouplingOperation<FITNESS_TYPE>>> _couplingOperation;

    boost::optional<std::shared_ptr<BaseCrossoverOperation<FITNESS_TYPE>>> _crossoverOperation;

    boost::optional<std::shared_ptr<BaseReplacementOperation<FITNESS_TYPE>>> _replacementOperation;

    boost::optional<std::shared_ptr<BaseMutationOperation<FITNESS_TYPE>>> _mutationOperation;

    boost::optional<std::shared_ptr<BaseChromosomeFactory<FITNESS_TYPE>>> _chromosomeFactory;

public:
    Builder()
    {
    }

    virtual ~Builder()
    {
    }

    Builder& setStoppingCriterion( const std::shared_ptr<stopping_criteria::BaseStoppingCriterion<FITNESS_TYPE>> &stoppingCriterion )
    {
        this->_stoppingCriterion = stoppingCriterion;
        return *this;
    }

    Builder& setSelectionOperation( const std::shared_ptr<BaseSelectionOperation<FITNESS_TYPE>> &selectionOperation )
    {
        this->_selectionOperation = selectionOperation;
        return *this;
    }

    Builder& setCouplingOperation( const std::shared_ptr<BaseCouplingOperation<FITNESS_TYPE>> &couplingOperation )
    {
        this->_couplingOperation = couplingOperation;
        return *this;
    }

    Builder& setCrossoverOperation( const std::shared_ptr<BaseCrossoverOperation<FITNESS_TYPE>> &crossoverOperation )
    {
        this->_crossoverOperation = crossoverOperation;
        return *this;
    }

    Builder& setReplacementOperation( const std::shared_ptr<BaseReplacementOperation<FITNESS_TYPE>> &replacementOperation )
    {
        this->_replacementOperation = replacementOperation;
        return *this;
    }

    Builder& setMutationOperation( const std::shared_ptr<BaseMutationOperation<FITNESS_TYPE>> &mutationOperation )
    {
        this->_mutationOperation = mutationOperation;
        return *this;
    }

    Builder& setChromosomeFactory (const std::shared_ptr<BaseChromosomeFactory<FITNESS_TYPE>> chromosomeFactory )
    {
        _chromosomeFactory = chromosomeFactory;
        return *this;
    }
};

} /* geneial_export_namespace */
} /* private namespace algorithm */
} /* private namespace geneial */


#include <geneial/algorithm/BaseGeneticAlgorithm.hpp>

