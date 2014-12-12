/*
 * BasicChromome.h
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef BASE_CHROMOSOME_H_
#define BASE_CHROMOSOME_H_

#define CHROMOSOME_AGE_UNITIALIZED (0)

#include <geneial/utility/random.h>
#include <geneial/utility/printable.h>
#include <geneial/core/fitness/Fitness.h>

#include <iostream>
#include "boost/shared_ptr.hpp"

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

/**
 * @brief Abstract superclass for any type of chromosome
 */
template <typename FITNESS_TYPE>
class BaseChromosome : public Utility::printable {

public:
	typedef unsigned int chromosome_age;
	typedef typename boost::shared_ptr<BaseChromosome <FITNESS_TYPE> > ptr;
	typedef typename boost::shared_ptr<const BaseChromosome <FITNESS_TYPE> > const_ptr;

	/**
	 * Creates a new Chromosome with random values and a fitness of -1
	 */
	BaseChromosome() : _fitness(), _age(CHROMOSOME_AGE_UNITIALIZED) {};
	virtual ~BaseChromosome() {};


	/**
	 * Gets the fitness value of a Chromosome.
	 * Does not calculate the fitness.
	 */
	const typename Fitness<FITNESS_TYPE>::ptr getFitness() const;
	/**
	 * Sets Fitness of a chromosome
	 */
	void setFitness(const typename Fitness<FITNESS_TYPE>::ptr& fitness);

	/**
	 * Used to 'age' a chromosome. Increments the age of a chromosome by one
	 * Returns the new age of the chromosome
	 */
	chromosome_age doAge();

	/**
	 * Returns the age of a chromosome
	 */
	chromosome_age getAge() const;

	void setAge(const chromosome_age age);

	virtual void print(std::ostream& os) const = 0;

	const bool hasFitness() const{
		return !(_fitness == NULL);
	};

private:
	typename Fitness<FITNESS_TYPE>::ptr  _fitness;
	chromosome_age _age;
};

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#include <geneial/core/population/chromosome/BaseChromosome.hpp>

#endif /* BASE_CHROMOSOME_H_ */
