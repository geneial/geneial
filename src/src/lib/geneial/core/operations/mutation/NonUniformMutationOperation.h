/*
 * NonUniformMutationOperator.h
 *
 *  Created on: Dec 28, 2014
 *      Author: lukas
 */

#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_NONUNIFORMMUTATIONOPERATION_H_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_NONUNIFORMMUTATIONOPERATION_H_

#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include "assert.h"

namespace GeneticLibrary {
namespace Operation {
namespace Mutation {

using namespace GeneticLibrary::Population::Manager;

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class NonUniformMutationOperation: public BaseMutationOperation<FITNESS_TYPE> {

private:
	MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> * _builderSettings;
	MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> *_builderFactory;

	double _minimumModification;
	unsigned int _affectedGenerations;
public:
	/*
	 * NonUniformMutationOperation Mutates a chromosome, by modificating some of it's values with weight w.
	 * W decreases with the amount of mutations defined by affectedGenerations.
	 * minimumModification defines the percetual change to a value AFTER the number of affected Generations has been reached
	 *
	 * @param affectedGenerations defines how many Generations will be target for decreasing amounts of mutation
	 * @param minimumModification defines the lowest percentage of mutation that is possible
	 */

	NonUniformMutationOperation(
			unsigned int affectedGenerations,
			double minimumModification,
			MutationSettings *settings,
			BaseChoosingOperation<FITNESS_TYPE> *choosingOperation,
			MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> *builderSettings,
			MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> *builderFactory) :
			BaseMutationOperation<FITNESS_TYPE>(settings,choosingOperation),
					_builderSettings(builderSettings),
					_builderFactory(builderFactory) {
		//assert(minimumModification <= 1 && minimumModification >= 0);
		_minimumModification = minimumModification;
		_affectedGenerations = affectedGenerations;
		assert(_builderSettings != NULL);
		assert(_builderFactory != NULL);
	}
	;
	virtual ~NonUniformMutationOperation() {
	}
	;
	/*
	 *  Returns a new chromosome which is a partially mutated version of the old one.
	 *  */
	virtual typename Population::Population<FITNESS_TYPE>::chromosome_container doMutate(
			typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container mutants,
			BaseManager<FITNESS_TYPE> &manager
			);

	MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>* const & getBuilderFactory() const {
		return _builderFactory;
	}

	double getMinimumModification() const {
		return _minimumModification;
	}

	void setMinimumModification(double minimumModification) {
		_minimumModification = minimumModification;
	}

	unsigned int getAffectedGenerations() const {
		return _affectedGenerations;
	}

	void setAffectedGenerations(unsigned int affectedGenerations) {
		this->affectedGenerations = affectedGenerations;
	}
};
//class
} // namespace Mutation
} // namespace Operation
} // namespace GeneticLibrary

#include <geneial/core/operations/mutation/NonUniformMutationOperation.hpp>
#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_NONUNIFORMMUTATIONOPERATION_H_ */
