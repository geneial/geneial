#pragma once

#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/population/Population.h>
#include <geneial/namespaces.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(population)
    {
        using ::geneial::Fitness;
        using ::geneial::population::chromosome::BaseChromosome;
        using ::geneial::population::management::BaseManager;

        geneial_export_namespace
        {

            template <typename FITNESS_TYPE>
            Population<FITNESS_TYPE>::Population()
                : _fitnessMap(), _hashMap(),
                  //        _fitnessCache(),
                  _age(0)
            {
            }

            template <typename FITNESS_TYPE> Population<FITNESS_TYPE>::~Population() {}

            template <typename FITNESS_TYPE> void Population<FITNESS_TYPE>::print(std::ostream & os) const
            {
                os << "Population: Age (" << _age << "), #Chromosomes (" << _fitnessMap.size() << "):" << std::endl;
                os << "  Chromosomes" << std::endl;

                std::for_each(_fitnessMap.cbegin(), _fitnessMap.cend(),
                              [&os](const typename Population<FITNESS_TYPE>::fitnessmap_value_type &c)
                              { os << *(c.second); });
            }

            /**
             * @return The amount of chomosones which are currently within the population.
             */
            template <typename FITNESS_TYPE>
            typename Population<FITNESS_TYPE>::population_size Population<FITNESS_TYPE>::getSize() const
            {
                return _fitnessMap.size();
            }

            template <typename FITNESS_TYPE> unsigned int Population<FITNESS_TYPE>::getAge() const
            {
                return _age;
            }

            template <typename FITNESS_TYPE> void Population<FITNESS_TYPE>::setAge(unsigned int age)
            {
                _age = age;
            }

            /**
             * Increments the age of the current population by 1
             */
            template <typename FITNESS_TYPE> void Population<FITNESS_TYPE>::doAge()
            {
                // For each Chromosome increment age
                std::for_each(_hashMap.cbegin(), _hashMap.cend(),
                              [](const typename Population<FITNESS_TYPE>::hashmap_value_type &c)
                              { c.second->doAge(); });

                // Age the population itself
                ++_age;
            }

            /**
             * Removes any duplicates from within the container, which are
             *     - appearing twice in the container
             *     - appearing already in the population
             *
             * Genome equality resolves hash collisions. Age and fitness are ignored.
             */
            template <typename FITNESS_TYPE>
            inline unsigned int Population<FITNESS_TYPE>::removeDuplicates(chromosome_container & toCheck)
            {
                const auto previousSize = toCheck.size();
                hash_map candidates;
                const auto end =
                    std::remove_if(toCheck.begin(), toCheck.end(),
                                   [&](const auto &chromosome)
                                   {
                                       if (!chromosome)
                                           throw std::invalid_argument("Chromosome cannot be null");
                                       const auto hashValue = chromosome->getHash();
                                       const auto range = candidates.equal_range(hashValue);
                                       const bool repeated = std::any_of(
                                           range.first, range.second, [&](const auto &entry)
                                           { return entry.second == chromosome || entry.second->equals(*chromosome); });
                                       if (repeated || containsChromosome(chromosome))
                                           return true;
                                       candidates.emplace(hashValue, chromosome);
                                       return false;
                                   });
                toCheck.erase(end, toCheck.end());
                return static_cast<unsigned int>(previousSize - toCheck.size());
            }

            template <typename FITNESS_TYPE>
            typename BaseChromosome<FITNESS_TYPE>::ptr Population<FITNESS_TYPE>::getOldestChromosome()
            {
                typename BaseChromosome<FITNESS_TYPE>::ptr oldest;
                for (const auto &fmv : _fitnessMap)
                {
                    if (!oldest)
                    {
                        oldest = fmv.second;
                    }
                    else
                    {
                        if (oldest->getAge() < fmv.second->getAge())
                        {
                            oldest = fmv.second;
                        }
                    }
                }
                return oldest;
            }

            template <typename FITNESS_TYPE>
            typename BaseChromosome<FITNESS_TYPE>::ptr Population<FITNESS_TYPE>::getYoungestChromosome()
            {
                typename BaseChromosome<FITNESS_TYPE>::ptr youngest;
                for (const auto &fmv : _fitnessMap)
                {
                    if (!youngest)
                    {
                        youngest = fmv.second;
                    }
                    else
                    {
                        if (youngest->getAge() > fmv.second->getAge())
                        {
                            youngest = fmv.second;
                        }
                    }
                }
                return youngest;
            }

            template <typename FITNESS_TYPE>
            inline bool Population<FITNESS_TYPE>::insertChromosome(
                const typename BaseChromosome<FITNESS_TYPE>::ptr &chromosome)
            {
                if (!containsChromosome(chromosome))
                {
                    _insertChromosome(chromosome, chromosome->getHash());
                    return true;
                }
                else
                {
                    return false;
                }
            }

            template <typename FITNESS_TYPE>
            inline void Population<FITNESS_TYPE>::_insertChromosome(
                const typename BaseChromosome<FITNESS_TYPE>::ptr &chromosome,
                typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue)
            {
                const auto fitness = chromosome->getFitness().get();
                if (!std::isfinite(static_cast<long double>(fitness)))
                    throw std::invalid_argument("Population fitness must be finite");
                const auto fitnessEntry = _fitnessMap.emplace(fitness, chromosome);
                try
                {
                    _hashMap.emplace(hashValue, chromosome);
                }
                catch (...)
                {
                    _fitnessMap.erase(fitnessEntry);
                    throw;
                }
            }

            template <typename FITNESS_TYPE>
            inline unsigned int Population<FITNESS_TYPE>::insertChromosomeContainer(chromosome_container & container)
            {
                removeDuplicates(container);
                for (const auto &chromosome : container)
                {
                    if (!chromosome->hasFitness())
                        getManager().getExecutionManager().addTask([chromosome] { chromosome->getFitness(); });
                }
                getManager().getExecutionManager().waitForTasks();
                for (const auto &chromosome : container)
                {
                    if (!std::isfinite(static_cast<long double>(chromosome->getFitness().get())))
                        throw std::invalid_argument("Population fitness must be finite");
                }
                for (const auto &chromosome : container)
                    _insertChromosome(chromosome, chromosome->getHash());
                return static_cast<unsigned int>(container.size());
            }

            template <typename FITNESS_TYPE>
            inline void Population<FITNESS_TYPE>::removeChromosome(
                const typename BaseChromosome<FITNESS_TYPE>::ptr &chromosome)
            {
                if (!chromosome)
                    throw std::invalid_argument("Chromosome cannot be null");
                const auto hashRange = _hashMap.equal_range(chromosome->getHash());
                const auto hashEntry = std::find_if(hashRange.first, hashRange.second, [&chromosome](const auto &entry)
                                                    { return entry.second == chromosome; });
                if (hashEntry == hashRange.second)
                    throw std::invalid_argument("Chromosome is not a population member");
                const auto fitnessRange = _fitnessMap.equal_range(chromosome->getFitness().get());
                const auto fitnessEntry =
                    std::find_if(fitnessRange.first, fitnessRange.second,
                                 [&chromosome](const auto &entry) { return entry.second == chromosome; });
                if (fitnessEntry == fitnessRange.second)
                    throw std::logic_error("Indexed chromosome fitness was modified");
                _fitnessMap.erase(fitnessEntry);
                _hashMap.erase(hashEntry);
            }

            template <typename FITNESS_TYPE>
            inline bool Population<FITNESS_TYPE>::hashExists(
                const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue) const
            {
                return _hashMap.find(hashValue) != _hashMap.end();
            }

            template <typename FITNESS_TYPE>
            inline bool Population<FITNESS_TYPE>::containsChromosome(
                const typename BaseChromosome<FITNESS_TYPE>::const_ptr &chromosome) const
            {
                if (!chromosome)
                    throw std::invalid_argument("Chromosome cannot be null");
                const auto range = _hashMap.equal_range(chromosome->getHash());
                return std::any_of(range.first, range.second, [&chromosome](const auto &entry)
                                   { return entry.second == chromosome || entry.second->equals(*chromosome); });
            }

            template <typename FITNESS_TYPE>
            inline typename BaseChromosome<FITNESS_TYPE>::ptr Population<FITNESS_TYPE>::getChromosomeByHash(
                const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue) const
            {
                const auto entry = _hashMap.find(hashValue);
                return entry == _hashMap.end() ? nullptr : entry->second;
            }

            template <typename FITNESS_TYPE>
            inline void Population<FITNESS_TYPE>::replacePopulation(chromosome_container & replacementPopulation)
            {
                Population staged;
                staged._manager = _manager;
                auto candidates = replacementPopulation;
                staged.insertChromosomeContainer(candidates);
                _fitnessMap.swap(staged._fitnessMap);
                _hashMap.swap(staged._hashMap);
                replacementPopulation.swap(candidates);
            }

            template <typename FITNESS_TYPE>
            inline void Population<FITNESS_TYPE>::removeChromosomeContainer(const chromosome_container &container)
            {
                for (const auto &chromosomomeToRemove : container)
                {
                    removeChromosome(chromosomomeToRemove);
                }
            }

            template <typename FITNESS_TYPE> inline void Population<FITNESS_TYPE>::clearChromosomes()
            {
                _fitnessMap.clear();
                _hashMap.clear();
            }

        } /* geneial_export_namespace */
    } /* private namespace population */
} /* private namespace geneial */
