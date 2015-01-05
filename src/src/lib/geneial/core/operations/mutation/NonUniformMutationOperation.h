/*
 * NonUniformMutationOperator.h
 *
 *  Created on: Dec 28, 2014
 *      Author: lukas
 */

#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_NONUNIFORMMUTATIONOPERATION_H_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_NONUNIFORMMUTATIONOPERATION_H_

#include <geneial/core/operations/mutation/BaseMutationOperation.h>

namespace GeneticLibrary {
namespace Operation {
namespace Mutation {

using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Operation::Mutation::BaseMutationOperation;

template <typename FITNESS_TYPE>
class NonUniformMutationOperation : public BaseMutationOperation<FITNESS_TYPE> {

public:
	/*
	 * UniformMutationOperation Mutates a chromosome, by replacing some of it's values randomly.
	 */
	NonUniformMutationOperation(MutationSettings *settings): BaseMutationOperation<FITNESS_TYPE>(settings){};
	virtual ~NonUniformMutationOperation() {};

	virtual typename BaseMutationOperation<FITNESS_TYPE>::mutation_result_set doMutate(Population::Chromosome::BaseChromosome<FITNESS_TYPE>::ptr mutant);

};

} // namespace Mutation
} // namespace Operation
} // namespace GeneticLibrary


#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_NONUNIFORMMUTATIONOPERATION_H_ */
