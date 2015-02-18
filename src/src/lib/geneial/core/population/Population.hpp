/*
 * Population.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <geneial/core/population/Population.h>
#include <map>

namespace GeneticLibrary {
namespace Population {

/**
 * Creates a vector containing ONE chromosome, the population needs to be build with replenishPopulation
 * @see replenishPopulation
 */
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
	for(typename chromosome_map::const_iterator chrom_it = _fitnessMap.begin(); chrom_it != _fitnessMap.end(); ++chrom_it)
	{
		os << *(chrom_it->second);
	}
}


/**
 *
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
	for (typename chromosome_map::iterator chrom_it =
			_fitnessMap.begin(); chrom_it != _fitnessMap.end(); ++chrom_it)
	{
		chrom_it->second->doAge();
	}
	++_age;
}

template<typename FITNESS_TYPE>
inline void Population<FITNESS_TYPE>::insertChromosome(typename BaseChromosome<FITNESS_TYPE>::ptr chromosome)
{
	//Insert into fitness map
	container_value fitness_map_value(chromosome->getFitness()->get(),chromosome);
	_fitnessMap.insert(fitness_map_value);

	//Insert into hash map
	typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue = chromosome->getHash();
	if(_hashMap.find(hashValue) != _hashMap.end()){
		std::cout <<"HASH COLLISION:" << hashValue << std::endl;
		std::cout <<"CHROMOSOME TO INSERT:" ;
		chromosome->print(std::cout);
		std::cout <<"CHROMOSOME CONTAINED:" ;
		typename hash_map::iterator it = _hashMap.find(hashValue);
		it->second->print(std::cout);
	}
	assert(_hashMap.find(hashValue) == _hashMap.end()); //NOTE (bewo): Without duplicate removal this check is superflous
	hashmap_value hash_map_value(hashValue,chromosome);
	_hashMap.insert(hash_map_value);
}

template<typename FITNESS_TYPE>
inline void Population<FITNESS_TYPE>::insertChromosomeContainer(const chromosome_container &container)
{
	  for (typename chromosome_container::const_iterator it = container.begin() ; it != container.end(); ++it)
	  {
		  this->insertChromosome(*it);
	  }
}


template<typename FITNESS_TYPE>
inline void Population<FITNESS_TYPE>::removeChromosome(typename BaseChromosome<FITNESS_TYPE>::ptr chromosome)
{
	const FITNESS_TYPE fitness = chromosome->getFitness()->get();
	const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hash = chromosome->getHash();


	std::pair <typename chromosome_map::iterator , typename chromosome_map::iterator > ret = _fitnessMap.equal_range(fitness);
	bool found = false;
	typename chromosome_map::iterator it;
	assert(ret.first != _fitnessMap.end());
	//we might have multiple chromosomes with the same key, advance until pointer is found
	for (it=ret.first; it!=ret.second; ++it)
	{
		  if(it->second == chromosome){
			  found = true;
			  break;
		  }
	}
	assert(found);
	typename hash_map::iterator hit = _hashMap.find(hash);
	assert(hit != _hashMap.end());
	_fitnessMap.erase(it);
	_hashMap.erase(hit);
	//assert(_fitnessMap.find(hash) == _fitnessMap.end());
	//assert(_hashMap.find(hash) == _hashMap.end());

}


template<typename FITNESS_TYPE>
inline bool Population<FITNESS_TYPE>::hashExists(const typename Chromosome::BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue)
{
	return (_hashMap.find(hashValue) != _hashMap.end());
}

template<typename FITNESS_TYPE>
inline typename Chromosome::BaseChromosome<FITNESS_TYPE>::ptr Population<FITNESS_TYPE>::getChromosomeByHash
	(const typename Chromosome::BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue)
{
	typename hash_map::iterator it = _hashMap.find-(hashValue);
	if(it == _hashMap.end()){
		typename Chromosome::BaseChromosome<FITNESS_TYPE>::ptr null_ptr;
		return (null_ptr);
	}else{
		return *it;
	}
}



template<typename FITNESS_TYPE>
inline void Population<FITNESS_TYPE>::replacePopulation(const chromosome_container &replacementPopulation)
{
	_fitnessMap.clear();
	_hashMap.clear();
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

} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* POPULATION_HPP_ */
