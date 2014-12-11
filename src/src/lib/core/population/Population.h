/*
 * Population.h
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include <utility/printable.h>
#include <core/population/chromosome/BaseChromosome.h>
#include <vector>

//TODO (bewo) chromosome_age is unsigned int but -1?
#define POPULATION_AGE_INITIAL (0)

using namespace GeneticLibrary::Utility;

namespace GeneticLibrary {
namespace Population {

template <typename FITNESS_TYPE>
class Population : public printable {
public:
	typedef unsigned int population_age;
	typedef unsigned int population_size;

	typedef typename std::vector<Chromosome::BaseChromosome<FITNESS_TYPE>* > chromosome_container;
	typedef typename chromosome_container::const_iterator const_it;
	typedef typename chromosome_container::iterator it;

	population_size getSize() const;

	Population();
	virtual ~Population();
	virtual void print(std::ostream& os) const;
	population_age getAge() const;
	void setAge(population_age age);
	void doAge();

	const chromosome_container& getChromosomes() const { return _chromosomes; }
	chromosome_container& getChromosomes() { return _chromosomes; }

private:
	chromosome_container _chromosomes;
	population_age _age;
};

} /* namespace Population */
} /* namespace GeneticLibrary */

#include <core/population/Population.hpp>

#endif /* POPULATION_H_ */
