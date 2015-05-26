#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/core/population/Population.h>
#include <geneial/core/population/PopulationSettings.h>
#include <geneial/core/population/builder/BaseChromosomeFactory.h>
#include <geneial/utility/ExecutionManager.h>

#include <cassert>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(management)
{
using ::geneial::population::chromosome::BaseChromosomeFactory;
using ::geneial::utility::BaseExecutionManager;
using ::geneial::utility::SequentialExecutionManager;

geneial_export_namespace
{

/**
 * Manages the population and most of the processes which modify the chromosomes
 */
template<typename FITNESS_TYPE>
class BaseManager : public std::enable_shared_from_this<BaseManager<FITNESS_TYPE> >
{
private:
    explicit BaseManager<FITNESS_TYPE>(const std::shared_ptr<BaseChromosomeFactory<FITNESS_TYPE>> &chromosomeFactory) :
        _holdoffSet(),
        _populationSettings(),
        _population(),
        _chromosomeFactory(chromosomeFactory), _executionManager(new SequentialExecutionManager())
        {
        }
    BaseManager(const BaseManager& other) = delete;
    BaseManager(const BaseManager&& other) = delete;
public:

    static std::shared_ptr<BaseManager<FITNESS_TYPE>> create(const std::shared_ptr<BaseChromosomeFactory<FITNESS_TYPE>> chromosomeFactory)
    {
        std::shared_ptr<BaseManager<FITNESS_TYPE>> ptr(new BaseManager<FITNESS_TYPE>(chromosomeFactory));

        ptr->_chromosomeFactory->setManager(ptr);
        ptr->_population._manager = ptr;

        return ptr;
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

    void deleteOrHoldOffReference(const typename BaseChromosome<FITNESS_TYPE>::ptr& chromosome)
    {
        if (_holdoffSet.size() <= _populationSettings.getHoldoffSize())
        {
            _holdoffSet.push_back(chromosome);
        }
    }

    typename BaseChromosome<FITNESS_TYPE>::ptr retrieveFromHoldOff()
    {
        typename BaseChromosome<FITNESS_TYPE>::ptr ptrCandidate;

        if (_holdoffSet.size() > 1)
        {
            ptrCandidate = _holdoffSet.back();
            _holdoffSet.pop_back();
        }

        return std::move(ptrCandidate);
    }

private:
    //Set for deleted chromosomes to reassign to factory
    typename Population<FITNESS_TYPE>::chromosome_container _holdoffSet;

    PopulationSettings _populationSettings;

    Population<FITNESS_TYPE> _population;

    std::shared_ptr<BaseChromosomeFactory<FITNESS_TYPE>> _chromosomeFactory;

    std::unique_ptr<BaseExecutionManager> _executionManager;

};

} /* geneial_export_namespace */
} /* private namespace management */
} /* private namespace population */
} /* private namespace geneial */


#include <geneial/core/population/management/BaseManager.hpp>
