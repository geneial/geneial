#ifndef __GENEIAL_POPULATION_HPP_
#define __GENEIAL_POPULATION_HPP_

#include <geneial/core/population/Population.h>
#include <geneial/core/fitness/Fitness.h>

#include <cassert>
#include <iostream>
#include <set>
#include <utility>

namespace geneial {
namespace population {

template <typename FITNESS_TYPE>
Population<FITNESS_TYPE>::Population(): _age(0) {}

template <typename FITNESS_TYPE>
Population<FITNESS_TYPE>::~Population() {}

template <typename FITNESS_TYPE>
void Population<FITNESS_TYPE>::print(std::ostream& os) const
{
	os << "Population: Age (" << _age << "), #Chromosomes ("<< _fitnessMap.size() <<"):" << std::endl;
	os << "  Chromosomes" << std::endl;

	//TODO (bewo) maybe use outstream iterator instead here.
	for(typename fitness_map::const_iterator chrom_it = _fitnessMap.begin(); chrom_it != _fitnessMap.end(); ++chrom_it)
	{
		os << *(chrom_it->second);
	}
}


/**
 * @return The amount of chomosones wich are part of the population.
 */
template <typename FITNESS_TYPE>
typename Population<FITNESS_TYPE>::population_size Population<FITNESS_TYPE>::getSize() const
{
	return _fitnessMap.size();
}


template <typename FITNESS_TYPE>
unsigned int Population<FITNESS_TYPE>::getAge() const
{
	return _age;
}

template <typename FITNESS_TYPE>
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
	for (typename fitness_map::iterator chrom_it =
			_fitnessMap.begin(); chrom_it != _fitnessMap.end(); ++chrom_it)
	{
		chrom_it->second->doAge();
	}
	++_age;
}

/**
 * Removes any duplicates from within the container, which are
 * 	- appearing twice in the container
 * 	- appearing already in the container
 *
 * 	NOTE: This function only considers a chromosome hash value,
 * 	and does not take the age into consideration.
 */
template<typename FITNESS_TYPE>
inline unsigned int Population<FITNESS_TYPE>::removeDuplicates(chromosome_container &toCheck)
{
	unsigned int removed = 0;
	typename chromosome_container::iterator it = toCheck.begin();
	std::set<typename BaseChromosome<FITNESS_TYPE>::chromsome_hash> tmpHashSet;

	for (; it != toCheck.end();)
	{
		const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue = (*it)->getHash();
		//Check whether hash is already contained in the container, or in the population
		if (tmpHashSet.find(hashValue) != tmpHashSet.end() || hashExists(hashValue))
		{
			it = toCheck.erase(it);
			removed++;
		} else {
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
	for(typename fitness_map::const_iterator chrom_it = _fitnessMap.begin(); chrom_it != _fitnessMap.end(); ++chrom_it)
	{
		if (!oldest)
		{
			oldest = chrom_it->second;
		}
		else
		{
			if(oldest->getAge() < chrom_it->second->getAge())
			{
				oldest = chrom_it->second;
			}
		}
	}
	return oldest;
}



template<typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr Population<FITNESS_TYPE>::getYoungestChromosome()
{
	typename BaseChromosome<FITNESS_TYPE>::ptr youngest;
	for(typename fitness_map::const_iterator chrom_it = _fitnessMap.begin(); chrom_it != _fitnessMap.end(); ++chrom_it)
	{
		if (!youngest)
		{
			youngest = chrom_it->second;
		}
		else
		{
			if(youngest->getAge() > chrom_it->second->getAge())
			{
				youngest = chrom_it->second;
			}
		}
	}
	return youngest;
}


template<typename FITNESS_TYPE>
inline bool Population<FITNESS_TYPE>::insertChromosome(typename BaseChromosome<FITNESS_TYPE>::ptr chromosome)
{
	//Insert into hash map
	typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue = chromosome->getHash();

	if(!hashExists(hashValue))
	{
		_insertChromosome(chromosome,hashValue);
		return true;
	}
	else
	{
		return false;
	}
}

template<typename FITNESS_TYPE>
inline void Population<FITNESS_TYPE>::_insertChromosome(typename BaseChromosome<FITNESS_TYPE>::ptr chromosome, typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue)
{
	assert(chromosome);
	//Insert into fitness map
	fitnessmap_value_type fitness_map_value(chromosome->getFitness()->get(),chromosome);
	_fitnessMap.insert(fitness_map_value);

	hashmap_value_type hash_map_value(hashValue,chromosome);
	_hashMap.insert(hash_map_value);
}

template<typename FITNESS_TYPE>
inline unsigned int Population<FITNESS_TYPE>::insertChromosomeContainer(chromosome_container &container)
{

	std::vector<typename BaseChromosome<FITNESS_TYPE>::chromsome_hash> hashCache;
	hashCache.reserve(container.size());

	for (typename chromosome_container::iterator it = container.begin() ; it != container.end();)
	{
		const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue = (*it)->getHash();
		const bool inHashCache = std::find(hashCache.begin(), hashCache.end(), hashValue) != hashCache.end();
		if(!hashExists(hashValue) && !inHashCache)
		{
			hashCache.push_back(hashValue);
			it++;
		}
		else
		{
			it = container.erase(it);
		}
	}

	//Use processing Strategy if not null, otherwise lazy evaluation will ensure Fitness at insertion
	if(processingStrategy)
	{
		processingStrategy->ensureHasFitness(container);
	}

	unsigned int i=0;
	for (typename chromosome_container::const_iterator it = container.begin() ; it != container.end(); ++it)
	{
		this->_insertChromosome(*it,hashCache[i]);
		i++;
	}
	return hashCache.size(); //TODO (bewo): This is a safe world assumption
}


template<typename FITNESS_TYPE>
inline void Population<FITNESS_TYPE>::removeChromosome(typename BaseChromosome<FITNESS_TYPE>::ptr chromosome)
{
	const FITNESS_TYPE fitness = chromosome->getFitness()->get();
	const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hash = chromosome->getHash();


	const std::pair <typename fitness_map::iterator , typename fitness_map::iterator > range = _fitnessMap.equal_range(fitness);
	bool candidateFound = false;

	typename fitness_map::iterator it = range.first;
	assert(range.first != _fitnessMap.end());

	//we might have multiple chromosomes with the same key, advance until pointer is candidateFound
	while(!candidateFound && it!=range.second)
	{
	    if(it->second == chromosome)
	    {
	        candidateFound = true;
	    }
	    else
	    {
	        ++it;
	    }
	}
	assert(candidateFound);
	typename hash_map::iterator hit = _hashMap.find(hash);
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
inline typename BaseChromosome<FITNESS_TYPE>::ptr Population<FITNESS_TYPE>::getChromosomeByHash
	(const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue)
{
	typename hash_map::iterator it = _hashMap.find-(hashValue);
	if(it == _hashMap.end())
	{
		typename BaseChromosome<FITNESS_TYPE>::ptr null_ptr;
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
	  for (typename chromosome_container::const_iterator it = container.begin() ; it != container.end(); ++it)
	  {
		  removeChromosome(*it);
	  }
}


template<typename FITNESS_TYPE>
inline void Population<FITNESS_TYPE>::clearChromosomes()
{
	_fitnessMap.clear();
    _hashMap.clear();
}

} /* namespace population */
} /* namespace geneial */

#endif /* __GENEIAL_POPULATION_HPP_ */
