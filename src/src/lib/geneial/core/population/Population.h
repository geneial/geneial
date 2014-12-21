/*
 * Population.h
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include <geneial/utility/printable.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <map>
#include <vector>

#define POPULATION_AGE_INITIAL (0)

using namespace GeneticLibrary::Utility;

namespace GeneticLibrary {
namespace Population {

template <typename FITNESS_TYPE>
class Population : public printable {
public:
	typedef unsigned int population_age;
	typedef unsigned int population_size;

	//A generic container to pass chromomsomes between operations.
	typedef typename std::vector<typename Chromosome::BaseChromosome<FITNESS_TYPE>::ptr> chromosome_container;

	//A fitness <-> chromsome map holding the actual population
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

	const chromosome_map& getChromosomes() const { return _chromosomes; }
	chromosome_map& getChromosomes() { return _chromosomes; }

	void insertChromosomeContainer(const chromosome_container &container);


private:
	chromosome_map _chromosomes;
	population_age _age;
};

} /* namespace Population */
} /* namespace GeneticLibrary */

#include <geneial/core/population/Population.hpp>

#endif /* POPULATION_H_ */
