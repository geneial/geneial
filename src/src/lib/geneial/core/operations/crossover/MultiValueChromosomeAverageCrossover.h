/*
 * MultiValueChromosomeAverageCrossover.h
 *
 *  Created on: Dec 12, 2014
 *      Author: bewo
 */

#ifndef MULTIVALUE_CHROMOSOME_AVERAGE_CROSSOVER_H_
#define MULTIVALUE_CHROMOSOME_AVERAGE_CROSSOVER_H_

#include "assert.h"
#include <geneial/core/operations/crossover/BaseCrossoverOperation.h>

namespace GeneticLibrary {
namespace Operation {
namespace Crossover {


template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeAverageCrossover: public BaseCrossoverOperation<FITNESS_TYPE> {
private:
	MultiValueBuilderSettings<VALUE_TYPE,FITNESS_TYPE> * _builderSettings;
	MultiValueChromosomeFactory<VALUE_TYPE,FITNESS_TYPE> *_builderFactory;

public:
	MultiValueChromosomeAverageCrossover(
			MultiValueBuilderSettings<VALUE_TYPE,FITNESS_TYPE> *builderSettings,
			MultiValueChromosomeFactory<VALUE_TYPE,FITNESS_TYPE> *builderFactory
			) :
			_builderSettings(builderSettings),
			_builderFactory(builderFactory)
			{
		assert(_builderSettings != NULL);
		assert(_builderFactory != NULL);

	}
	;
	virtual ~MultiValueChromosomeAverageCrossover() {
	}
	;
	/****
	 * Computes the average of the two parents.
	 *
	 * Example:
	 * Say, A and B are Chromosomes with
	 *
	 * A (mommy)
	 * ===========================
	 * |A1|A2|A3|A4|A5|A6|A7|A8|A9
	 * ===========================
	 *
	 * B (daddy)
	 * ===========================
	 * |B1|B2|B3|B4|B5|B6|B7|B8|B9
	 * ===========================
	 *
	 * Result is
	 *
	 * Child
	 * ===================================
	 * |(A1+B1)/2|(A2+B2)/2| ... (A9+B9)/2
	 * ===================================
	 *
	 */
	virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set
	doCrossover(typename BaseChromosome<FITNESS_TYPE>::ptr mommy,
			typename BaseChromosome<FITNESS_TYPE>::ptr daddy);

	MultiValueBuilderSettings<VALUE_TYPE,FITNESS_TYPE>* const & getBuilderSettings() const {
		return _builderSettings;
	}

	void setBuilderSettings(
			const MultiValueBuilderSettings<VALUE_TYPE,FITNESS_TYPE>* & builderSettings) {
		_builderSettings = builderSettings;
	}

	MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>* const & getBuilderFactory() const {
		return _builderFactory;
	}

	void setBuilderFactory(
			const MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>* & builderFactory) {
		_builderFactory = builderFactory;
	}

};


} /* namespace Crossover */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#include <geneial/core/operations/crossover/MultiValueChromosomeAverageCrossover.hpp>

#endif /* MULTIVALUE_CHROMOSOME_AVERAGE_CROSSOVER_H_ */
