/*
 * UniformMutationOperation.h
 *
 *  Created on: Dec 28, 2014
 *      Author: lukas
 */

#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_UNIFORMMUTATIONOPERATION_H_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_UNIFORMMUTATIONOPERATION_H_

#include <geneial/core/operations/mutation/BaseMutationOperation.h>

namespace GeneticLibrary {
namespace Operation {
namespace Mutation {

using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Operation::Mutation::BaseMutationOperation;

template <typename FITNESS_TYPE>
class UniformMutationOperation : public BaseMutationOperation<FITNESS_TYPE> {

public:
	UniformMutationOperation(MutationSettings *settings): BaseMutationOperation<FITNESS_TYPE>(settings){};
	virtual ~UniformMutationOperation() {};

	virtual typename BaseMutationOperation<FITNESS_TYPE>::mutation_result_set doMutate(BaseChromosome<FITNESS_TYPE>::ptr mutant);

};

} // namespace Mutation
} // namespace Operation
} // namespace GeneticLibrary



#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_UNIFORMMUTATIONOPERATION_H_ */
