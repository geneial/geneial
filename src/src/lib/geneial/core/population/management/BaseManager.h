#pragma once

#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/core/population/Population.h>
#include <geneial/core/population/PopulationSettings.h>
#include <geneial/core/population/builder/BaseChromosomeFactory.h>
#include <geneial/utility/ExecutionManager.h>

#include <cassert>

namespace geneial
{
namespace population
{
namespace management
{

/**
 * Manages the population and most of the processes which modify the chromosomes
 */
template<typename FITNESS_TYPE>
class BaseManager : public std::enable_shared_from_this<BaseManager<FITNESS_TYPE> >
{

public:
    explicit BaseManager<FITNESS_TYPE>(std::shared_ptr<BaseChromosomeFactory<FITNESS_TYPE>> chromosomeFactory) :
        _populationSettings(),
        _population(*this),
        _chromosomeFactory(chromosomeFactory), _executionManager(new SequentialExecutionManager())
    {
    }

    virtual ~BaseManager<FITNESS_TYPE>()
    {
    }

    void replacePopulation(typename Population<FITNESS_TYPE>::chromosome_container replacementPopulation);

    /**
     * Adds new chromosomes to the population until a limit is reached
     */
    void replenishPopulation();

    typename BaseChromosome<FITNESS_TYPE>::ptr getHighestFitnessChromosome() const;

    FITNESS_TYPE getHighestFitness() const;

    typename BaseChromosome<FITNESS_TYPE>::ptr getLowestFitnessChromosome() const;

    FITNESS_TYPE getLowestFitness() const;

    Population<FITNESS_TYPE>& getPopulation()
    {
        return _population;
    }

    Population<FITNESS_TYPE>& getPopulation() const
    {
        return const_cast<Population<FITNESS_TYPE>&>(_population);
    }

    BaseChromosomeFactory<FITNESS_TYPE>& getChromosomeFactory() const
    {
        return _chromosomeFactory;
    }

    void setChromosomeFactory(std::shared_ptr<BaseChromosomeFactory<FITNESS_TYPE>> chromosomeFactory)
    {
        _chromosomeFactory = chromosomeFactory;
    }

    PopulationSettings& getPopulationSettings()
    {
        return _populationSettings;
    }

    void setPopulationSettings(PopulationSettings& populationSettings)
    {
        _populationSettings = populationSettings;
    }

    BaseExecutionManager& getExecutionManager() const
    {
        return *_executionManager;
    }

    void setExecutionManager(std::unique_ptr<BaseExecutionManager> executionManager)
    {
        _executionManager = std::move(executionManager);
    }

private:
    PopulationSettings _populationSettings;

    Population<FITNESS_TYPE> _population;

    std::shared_ptr<BaseChromosomeFactory<FITNESS_TYPE>> _chromosomeFactory;

    std::unique_ptr<BaseExecutionManager> _executionManager;

};

} /* namespace manager */
} /* namespace population */
} /* namespace geneial */

#include <geneial/core/population/management/BaseManager.hpp>
