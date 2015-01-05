/*
 * NonUniformMutationOperation.hpp
 *
 *  Created on: Dec 30, 2014
 *      Author: lukas
 */

#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_NONUNIFORMMUTATIONOPERATION_HPP_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_NONUNIFORMMUTATIONOPERATION_HPP_

#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/operations/mutation/NonUniformMutationOperation.h>

using namespace GeneticLibrary::Population::Chromosome;

namespace GeneticLibrary {
namespace Operation {
namespace Mutation {

template<typename FITNESS_TYPE>
typename BaseMutationOperation<FITNESS_TYPE>::mutation_result_set doMutate(double min, double max, typename BaseChromosome<FITNESS_TYPE>::ptr mutant){

//TODO Write awesome Non-Uniform Mutation-algorithm here.

};



} //namespace Mutation
} //namespace Operation
} //namespace GeneticLibrary





#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_NONUNIFORMMUTATIONOPERATION_HPP_ */
