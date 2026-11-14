#pragma once

#include <geneial/core/population/Population.h>
#include <geneial/core/population/management/BaseManager.h>

#include <iterator>
#include <stdexcept>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(population)
    {
        geneial_private_namespace(management)
        {

            geneial_export_namespace
            {

                /**
                 *  Manages the population and most of the processes which modify the chromosomes
                 */

                template <typename FITNESS_TYPE> void BaseManager<FITNESS_TYPE>::replenishPopulation()
                {
                    const auto originalSize = _population.getSize();
                    unsigned int failedAttempts = 0;

                    // if there are less chromosomes than required fill up:
                    while (_population.getSize() < _populationSettings.getMaxChromosomes())
                    {
                        // build new chromosome
                        const typename BaseChromosome<FITNESS_TYPE>::ptr newChromosome(
                            _chromosomeFactory->createChromosome());
                        if (_population.insertChromosome(newChromosome))
                        {
                            failedAttempts = 0;
                        }
                        else if (++failedAttempts >= _populationSettings.getMaxReplenishAttempts())
                        {
                            throw std::runtime_error("Cannot replenish population: too few distinct chromosomes; "
                                                     "reduce the population size or widen the search space");
                        }
                    }

                    EventValueData<unsigned int>::create(*this->getBookkeeper(), "REPLENISH_AMOUNT",
                                                         _population.getSize() - originalSize);
                }

                template <typename FITNESS_TYPE>
                void BaseManager<FITNESS_TYPE>::replacePopulation(
                    typename Population<FITNESS_TYPE>::chromosome_container replacementPopulation)
                {
                    _population.replacePopulation(replacementPopulation);
                }

                template <typename FITNESS_TYPE>
                typename BaseChromosome<FITNESS_TYPE>::ptr BaseManager<FITNESS_TYPE>::getHighestFitnessChromosome()
                    const
                {
                    if (_population.getFitnessMap().empty())
                    {
                        throw std::logic_error("Population is empty");
                    }
                    return _population.getFitnessMap().rbegin()->second;
                }

                template <typename FITNESS_TYPE> FITNESS_TYPE BaseManager<FITNESS_TYPE>::getHighestFitness() const
                {
                    if (_population.getFitnessMap().empty())
                    {
                        throw std::logic_error("Population is empty");
                    }
                    return _population.getFitnessMap().rbegin()->first;
                }

                template <typename FITNESS_TYPE>
                typename BaseChromosome<FITNESS_TYPE>::ptr BaseManager<FITNESS_TYPE>::getLowestFitnessChromosome() const
                {
                    if (_population.getFitnessMap().empty())
                    {
                        throw std::logic_error("Population is empty");
                    }
                    return _population.getFitnessMap().begin()->second;
                }

                template <typename FITNESS_TYPE> FITNESS_TYPE BaseManager<FITNESS_TYPE>::getLowestFitness() const
                {
                    if (_population.getFitnessMap().empty())
                    {
                        throw std::logic_error("Population is empty");
                    }
                    return _population.getFitnessMap().begin()->first;
                }

            } /* geneial_export_namespace */
        } /* private namespace management */
    } /* private namespace population */
} /* private namespace geneial */
