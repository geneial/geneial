#pragma once

#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/core/population/Population.h>
#include <geneial/core/population/PopulationSettings.h>
#include <geneial/core/population/builder/BaseChromosomeFactory.h>

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
class BaseManager
{

public:
    BaseManager<FITNESS_TYPE>(BaseChromosomeFactory<FITNESS_TYPE> &chromosomeFactory) :
        _population(),_chromosomeFactory(chromosomeFactory), _populationSettings()
    {
    }

    virtual ~BaseManager<FITNESS_TYPE>()
    {
    }


    static long int amountRepl;


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

    void setChromosomeFactory(BaseChromosomeFactory<FITNESS_TYPE>*& chromosomeFactory)
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
private:
    Population<FITNESS_TYPE> _population;

    BaseChromosomeFactory<FITNESS_TYPE> &_chromosomeFactory;

    PopulationSettings _populationSettings;

};
template<typename FITNESS_TYPE>


} /* namespace manager */
} /* namespace population */
} /* namespace geneial */

#include <geneial/core/population/management/BaseManager.hpp>

