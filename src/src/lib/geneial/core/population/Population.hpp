#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/Population.h>
#include <geneial/core/fitness/Fitness.h>

#include <cassert>
#include <iostream>
#include <algorithm>
#include <set>
#include <utility>
#include <memory>


geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
using ::geneial::population::chromosome::BaseChromosome;
using ::geneial::population::management::BaseManager;
using ::geneial::Fitness;

geneial_export_namespace
{

template<typename FITNESS_TYPE>
Population<FITNESS_TYPE>::Population() :
        _fitnessMap(),
        _hashMap(),
//        _fitnessCache(),
        _age(0)
{
}

template<typename FITNESS_TYPE>
Population<FITNESS_TYPE>::~Population()
{
}

template<typename FITNESS_TYPE>
void Population<FITNESS_TYPE>::print(std::ostream& os) const
{
    os << "Population: Age (" << _age << "), #Chromosomes (" << _fitnessMap.size() << "):" << std::endl;
    os << "  Chromosomes" << std::endl;

    std::for_each (_fitnessMap.cbegin(), _fitnessMap.cend(), [&os](const typename Population<FITNESS_TYPE>::fitnessmap_value_type &c){
        os << *(c.second);
    });
}

/**
 * @return The amount of chomosones which are currently within the population.
 */
template<typename FITNESS_TYPE>
typename Population<FITNESS_TYPE>::population_size Population<FITNESS_TYPE>::getSize() const
{
    return _fitnessMap.size();
}

template<typename FITNESS_TYPE>
unsigned int Population<FITNESS_TYPE>::getAge() const
{
    return _age;
}

template<typename FITNESS_TYPE>
void Population<FITNESS_TYPE>::setAge(unsigned int age)
{
    _age = age;
}

/**
 * Increments the age of the current population by 1
 */
template<typename FITNESS_TYPE>
void Population<FITNESS_TYPE>::doAge()
{
    //For each Chromosome increment age
    std::for_each (_hashMap.cbegin(), _hashMap.cend(), [](const typename Population<FITNESS_TYPE>::hashmap_value_type &c){
        c.second->doAge();
    });

    //Age the population itself
    ++_age;
}

/**
 * Removes any duplicates from within the container, which are
 *     - appearing twice in the container
 *     - appearing already in the container
 *
 *     NOTE: This function only considers a chromosome hash value,
 *     and does not take the age into consideration.
 */
template<typename FITNESS_TYPE>
inline unsigned int Population<FITNESS_TYPE>::removeDuplicates(chromosome_container &toCheck)
{
    unsigned int removed = 0;
    std::set<typename BaseChromosome<FITNESS_TYPE>::chromsome_hash> tmpHashSet;

    for (auto it = toCheck.begin(); it != toCheck.end();)
    {
        const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue = (*it)->getHash();
        //Check whether hash is already contained in the container, or in the population
        if (tmpHashSet.find(hashValue) != tmpHashSet.end() || hashExists(hashValue))
        {
            it = toCheck.erase(it);
            removed++;
        }
        else
        {
            tmpHashSet.insert(hashValue);
            ++it;
        }
    }

    return removed;
}

template<typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr Population<FITNESS_TYPE>::getOldestChromosome()
{
    typename BaseChromosome<FITNESS_TYPE>::ptr oldest;
    for (const auto& fmv : _fitnessMap)
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

template<typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr Population<FITNESS_TYPE>::getYoungestChromosome()
{
    typename BaseChromosome<FITNESS_TYPE>::ptr youngest;
    for (const auto& fmv : _fitnessMap)
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

template<typename FITNESS_TYPE>
inline bool Population<FITNESS_TYPE>::insertChromosome(const typename BaseChromosome<FITNESS_TYPE>::ptr& chromosome)
{
    //Insert into hash map
    typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue = chromosome->getHash();
    if (!hashExists(hashValue))
    {
        _insertChromosome(chromosome, hashValue);
        return true;
    }
    else
    {
        return false;
    }
}

template<typename FITNESS_TYPE>
inline void Population<FITNESS_TYPE>::_insertChromosome(const typename BaseChromosome<FITNESS_TYPE>::ptr& chromosome,
        typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue)
{
//    const auto it = this->_fitnessCache.find(chromosome->getHash());

//    if(it != this->_fitnessCache.end())
//    {
//        chromosome->setFitness(std::move(std::unique_ptr<Fitness<FITNESS_TYPE>>(new Fitness<FITNESS_TYPE>(it->second))));
//    }

    // TODO(bewo) enable this by setting @{
//    _fitnessCache[hashValue] = chromosome->getFitness().get();
    // @}

    assert(chromosome);
    //Insert into fitness map
    fitnessmap_value_type fitness_map_value(chromosome->getFitness().get(), chromosome);
    _fitnessMap.insert(fitness_map_value);

    hashmap_value_type hash_map_value(hashValue, chromosome);
    _hashMap.insert(hash_map_value);
}

template<typename FITNESS_TYPE>
inline unsigned int Population<FITNESS_TYPE>::insertChromosomeContainer(chromosome_container &container)
{
    std::vector<typename BaseChromosome<FITNESS_TYPE>::chromsome_hash> hashCache;
    hashCache.reserve(container.size());
    for (typename chromosome_container::iterator it = container.begin(); it != container.end();)
    {
        const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue = (*it)->getHash();
        const bool inHashCache = std::find(hashCache.begin(), hashCache.end(), hashValue) != hashCache.end();
        if (!hashExists(hashValue) && !inHashCache)
        {
            hashCache.emplace_back(hashValue);
            it++;
        }
        else
        {
            //getManager().deleteOrHoldOffReference(*it);
            it = container.erase(it);
        }
    }

    for (size_t i = 0; i < container.size(); i++)
    {
        if (!container[i]->hasFitness())
        {
            getManager().getExecutionManager().addTask([i,&container]
            {
//                const auto it = this->_fitnessCache.find(chromosome->getHash());
//                if(it == this->_fitnessCache.end())
//                {
                    container[i]->getFitness().get();
//                }
//                else
//                {
//                    //std::cout << "fitness_cache hit"<<std::endl;
//                    //TODO (bewo) If the user defined Fitness Evaluator might choose to override Fitness, we have a problem here..
//                    chromosome->setFitness(std::move(std::unique_ptr<Fitness<FITNESS_TYPE>>(new Fitness<FITNESS_TYPE>(it->second))));
//                }
            });
        }
    }

    getManager().getExecutionManager().waitForTasks();

    unsigned int i = 0;
    std::for_each(container.cbegin(),container.cend(),
            [&i,this,&hashCache]
                (const typename chromosome_container::value_type &it)
            {
                this->_insertChromosome(it, hashCache[i]);
                ++i;
            }
    );
    return hashCache.size(); //TODO (bewo): This is a safe world assumption
}

template<typename FITNESS_TYPE>
inline void Population<FITNESS_TYPE>::removeChromosome(const typename BaseChromosome<FITNESS_TYPE>::ptr &chromosome)
{
    const FITNESS_TYPE fitness = chromosome->getFitness().get();

    const auto hash = chromosome->getHash();
    const auto range = _fitnessMap.equal_range(fitness);

    bool candidateFound = false;

    auto it = range.first;
    assert(range.first != _fitnessMap.end());

    //we might have multiple chromosomes with the same key, advance until pointer is candidateFound
    while (!candidateFound && it != range.second)
    {
        if (it->second == chromosome)
        {
            candidateFound = true;
        }
        else
        {
            ++it;
        }
    }

    assert(candidateFound);
    const auto hit = _hashMap.find(hash);
    assert(hit != _hashMap.end());

    _fitnessMap.erase(it);
    _hashMap.erase(hit);

    //assert(_fitnessMap.find(hash) == _fitnessMap.end());
    //assert(_hashMap.find(hash) == _hashMap.end());

}

template<typename FITNESS_TYPE>
inline bool Population<FITNESS_TYPE>::hashExists(const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue)
{
    return (_hashMap.find(hashValue) != _hashMap.end());
}

template<typename FITNESS_TYPE>
inline typename BaseChromosome<FITNESS_TYPE>::ptr Population<FITNESS_TYPE>::getChromosomeByHash(
        const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue)
{
    const auto it = _hashMap.find - (hashValue);
    if (it == _hashMap.end())
    {
        typename BaseChromosome<FITNESS_TYPE>::ptr null_ptr(nullptr);
        return (null_ptr);
    }
    else
    {
        return *it;
    }
}

template<typename FITNESS_TYPE>
inline void Population<FITNESS_TYPE>::replacePopulation(chromosome_container &replacementPopulation)
{
    clearChromosomes();
    insertChromosomeContainer(replacementPopulation);
}

template<typename FITNESS_TYPE>
inline void Population<FITNESS_TYPE>::removeChromosomeContainer(const chromosome_container &container)
{
    for(const auto& chromosomomeToRemove: container)
    {
        removeChromosome(chromosomomeToRemove);
    }
}

template<typename FITNESS_TYPE>
inline void Population<FITNESS_TYPE>::clearChromosomes()
{
    _fitnessMap.clear();
    _hashMap.clear();
}

} /* geneial_export_namespace */
} /* private namespace population */
} /* private namespace geneial */
