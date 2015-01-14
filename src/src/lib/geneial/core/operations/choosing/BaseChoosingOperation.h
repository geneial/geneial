/*
 * BaseChoosingOperation.h
 *
 *  Created on: Jan 12, 2015
 *      Author: lukas
 */

#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_BASECHOOSINGOPERATION_H_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_BASECHOOSINGOPERATION_H_

#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/population/Population.h>

namespace GeneticLibrary {
namespace Operation {
namespace Choosing {

using namespace GeneticLibrary::Operation::Mutation;

template <typename FITNESS_TYPE>
class BaseChoosingOperation{
private:

public:
	BaseChoosingOperation () {};
	virtual ~BaseChoosingOperation() {};

	virtual typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container doChoose (
			typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container chromosomeInputSet) = 0;
};

} //namespace Mutation
} //namespace Operation
} //namespace GenLib



#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_BASECHOOSINGOPERATION_H_ */
