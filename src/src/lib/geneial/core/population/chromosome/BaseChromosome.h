/*
 * BasicChromome.h
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef BASE_CHROMOSOME_H_
#define BASE_CHROMOSOME_H_

#define CHROMOSOME_AGE_UNITIALIZED (0)

#include <geneial/utility/Printable.h>
#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/fitness/FitnessEvaluator.h>

#include <iostream>
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

/**
 * @brief Abstract superclass for any type of chromosome
 */
template <typename FITNESS_TYPE>
class BaseChromosome : public Utility::Printable,
					   public boost::enable_shared_from_this<BaseChromosome <FITNESS_TYPE> > {

public:
	typedef unsigned int chromosome_age;
	typedef long chromsome_hash;

	typedef typename boost::shared_ptr<BaseChromosome <FITNESS_TYPE> > ptr;
	typedef typename boost::shared_ptr<const BaseChromosome <FITNESS_TYPE> > const_ptr;

	ptr getPtr() //TODO (bewo) constness correct?
    {
        return this->shared_from_this();
    };

	const_ptr getConstPtr() //TODO (bewo) constness correct?
    {
        return this->shared_from_this();
    };

	/**
	 * Creates a new Chromosome with random values and a fitness of -1
	 */
	BaseChromosome(typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator)
		: _fitness(),
		 _fitnessEvaluator(fitnessEvaluator),
		 _age(CHROMOSOME_AGE_UNITIALIZED) {
		assert(_fitnessEvaluator);
	};
	virtual ~BaseChromosome() {};


	virtual bool equals(const_ptr chromosome) const = 0;


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

	bool hasFitness() const{
		return !(_fitness == NULL);
	};

	/**
	 * Gets the fitness value of a Chromosome.
	 * Calls the fitness evaluator, if chromosome has no fitness yet.
	 */
	const typename Fitness<FITNESS_TYPE>::ptr getFitness();

	/**
	 * Gets the fitness value of a Chromosome.
	 * Does not evaluate the fitness.
	 */
	const typename Fitness<FITNESS_TYPE>::ptr getFitness() const;
	/**
	 * Sets Fitness of a chromosome
	 */
	void setFitness(const typename Fitness<FITNESS_TYPE>::ptr& fitness);



	bool hasFitnessEvaluator() const{
		return !(_fitnessEvaluator == NULL);
	}

	const typename FitnessEvaluator<FITNESS_TYPE>::ptr getFitnessEvaluator() const;

	void setFitnessEvaluator(const typename FitnessEvaluator<FITNESS_TYPE>::ptr& fitnessEvaluator);

	virtual chromsome_hash getHash() const = 0;
protected:
	virtual bool hashEquals(const_ptr chromosome) const;
	virtual void printHash(std::ostream& os) const;
private:
	typename Fitness<FITNESS_TYPE>::ptr  _fitness;

	typename FitnessEvaluator<FITNESS_TYPE>::ptr _fitnessEvaluator;

	chromosome_age _age;
};

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#include <geneial/core/population/chromosome/BaseChromosome.hpp>

#endif /* BASE_CHROMOSOME_H_ */
