/*
 * Population.h
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <iostream>
#include <map>
#include <vector>

#define POPULATION_AGE_INITIAL (0)

using namespace GeneticLibrary::Utility;

namespace GeneticLibrary {
namespace Population {

template <typename FITNESS_TYPE>
class Population : public printable
{
public:
	//TODO(bewo): cleanup this typedef mess
	typedef unsigned int population_age;
	typedef unsigned int population_size;

	//A generic container to pass chromomsomes between operations.
	typedef typename std::vector<typename Chromosome::BaseChromosome<FITNESS_TYPE>::ptr> chromosome_container;

	//A map containing all the chromosomes hash values.
	typedef typename std::map
			<typename Chromosome::BaseChromosome<FITNESS_TYPE>::chromsome_hash,
			 typename Chromosome::BaseChromosome<FITNESS_TYPE>::ptr> hash_map;

	//A hash <-> chromosome ptr map, hash value demands to be unique
	typedef typename hash_map::value_type hashmap_value;


	//A fitness <-> chromsome map holding the actual population, multiple chromosomes might have same fitness
	typedef typename std::multimap<FITNESS_TYPE , typename Chromosome::BaseChromosome<FITNESS_TYPE>::ptr> chromosome_map;
	typedef typename chromosome_map::value_type container_value;
	typedef typename chromosome_map::const_iterator const_it;
	typedef typename chromosome_map::iterator it;

	population_size getSize() const;

	Population();
	virtual ~Population();
	virtual void print(std::ostream& os) const;

	population_age getAge() const;
	void setAge(population_age age);
	void doAge();


	const inline chromosome_map& getChromosomes() const
	{
		return _fitnessMap;
	}

	const inline hash_map& getHashmap() const
	{
		return _hashMap;
	}

	bool hashExists(const typename Chromosome::BaseChromosome<FITNESS_TYPE>::chromsome_hash);

	unsigned int removeDuplicates(chromosome_container &toCheck);


	typename Chromosome::BaseChromosome<FITNESS_TYPE>::ptr getChromosomeByHash(const typename Chromosome::BaseChromosome<FITNESS_TYPE>::chromsome_hash);

	void replacePopulation(const chromosome_container &replacementPopulation);
	void insertChromosomeContainer(const chromosome_container &container);

	void insertChromosome(const typename BaseChromosome<FITNESS_TYPE>::ptr chromosome);

	void removeChromosomeContainer(const chromosome_container &container);
	void removeChromosome(const typename BaseChromosome<FITNESS_TYPE>::ptr chromosome);


private:
	chromosome_map _fitnessMap;
	hash_map _hashMap;
	population_age _age;
};

} /* namespace Population */
} /* namespace GeneticLibrary */

#include <geneial/core/population/Population.hpp>

#endif /* POPULATION_H_ */
