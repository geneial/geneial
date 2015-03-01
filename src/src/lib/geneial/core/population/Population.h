/*
 * Population.h
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/core/population/ContainerTypes.h>
#include <geneial/core/fitness/BaseFitnessProcessingStrategy.h>
#include <geneial/utility/Printable.h>

#include <iostream>
#include <map>
#include <vector>


#define POPULATION_AGE_INITIAL (0)


namespace GeneticLibrary {
namespace Population {

using namespace GeneticLibrary::Utility;

template <typename FITNESS_TYPE>
class Population : public Printable
{
public:
	//TODO(bewo): cleanup this typedef mess
	typedef unsigned int population_age;
	typedef unsigned int population_size;

	//Alias chromomsome container
	typedef typename ContainerTypes<FITNESS_TYPE>::chromosome_container chromosome_container;

	//A map containing all the chromosomes hash values.
	typedef typename std::map
			<typename Chromosome::BaseChromosome<FITNESS_TYPE>::chromsome_hash,
			 typename Chromosome::BaseChromosome<FITNESS_TYPE>::ptr> hash_map;

	typedef typename hash_map::value_type hashmap_value_type;
	typedef typename hash_map::key_type hashmap_key_type;

	typedef typename hash_map::const_iterator hashmap_const_it;
	typedef typename hash_map::iterator hashmap_it;



	//A fitness <-> chromsome map holding the actual population, multiple chromosomes might have same fitness
	typedef typename std::multimap<FITNESS_TYPE , typename Chromosome::BaseChromosome<FITNESS_TYPE>::ptr> fitness_map;

	typedef typename fitness_map::value_type fitnessmap_value_type;
	typedef typename fitness_map::key_type fitnessmap_key_type;

	typedef typename fitness_map::const_iterator fitnessmap_const_it;
	typedef typename fitness_map::iterator fitnessmap_it;



	population_size getSize() const;

	Population();
	virtual ~Population();
	virtual void print(std::ostream& os) const;

	population_age getAge() const;
	void setAge(population_age age);
	void doAge();

	typename Chromosome::BaseChromosome<FITNESS_TYPE>::ptr getOldestChromosome();

	typename Chromosome::BaseChromosome<FITNESS_TYPE>::ptr getYoungestChromosome();

	BaseFitnessProcessingStrategy<FITNESS_TYPE>* processingStrategy;


	const inline fitness_map& getFitnessMap() const
	{
		return _fitnessMap;
	}

	const inline hash_map& getHashMap() const
	{
		return _hashMap;
	}

	bool hashExists(const typename Chromosome::BaseChromosome<FITNESS_TYPE>::chromsome_hash);

	unsigned int removeDuplicates(chromosome_container &toCheck);


	typename Chromosome::BaseChromosome<FITNESS_TYPE>::ptr getChromosomeByHash(const typename Chromosome::BaseChromosome<FITNESS_TYPE>::chromsome_hash);

	void replacePopulation(chromosome_container &replacementPopulation);
	unsigned int insertChromosomeContainer(chromosome_container &container);

	bool insertChromosome(const typename BaseChromosome<FITNESS_TYPE>::ptr chromosome);

	void removeChromosomeContainer(const chromosome_container &container);
	void removeChromosome(const typename BaseChromosome<FITNESS_TYPE>::ptr chromosome);

	const BaseFitnessProcessingStrategy<FITNESS_TYPE>*& getProcessingStrategy() const {
		return processingStrategy;
	}

	void setProcessingStrategy(
			BaseFitnessProcessingStrategy<FITNESS_TYPE>*& processingStrategy) {
		this->processingStrategy = processingStrategy;
	}

private:

	void _insertChromosome(const typename BaseChromosome<FITNESS_TYPE>::ptr chromosome,typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue);

	fitness_map _fitnessMap;
	hash_map _hashMap;
	population_age _age;
};

} /* namespace Population */
} /* namespace GeneticLibrary */

#include <geneial/core/population/Population.hpp>

#endif /* POPULATION_H_ */
