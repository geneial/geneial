/*
 * ChooseRandom.h
 *
 *  Created on: Jan 12, 2015
 *      Author: lukas
 */

#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_CHOOSERANDOM_H_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_CHOOSERANDOM_H_

#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>

using namespace GeneticLibrary::Operation::Mutation;

namespace GeneticLibrary {
namespace Operation {
namespace Choosing {

template <typename VALUE_TYPE, typename FITNESS_TYPE>
class ChooseRandom : public BaseMutationOperation <FITNESS_TYPE> {
public:
	ChooseRandom
		(
			MutationSettings *settings
		): BaseMutationOperation<FITNESS_TYPE>(settings)
		{};
	virtual ~ChooseRandom(){};

	/*
	 * Chooses from an chromosomeContainer Random values. The propability that a Chromosome is choosen and therefore part of the
	 * resultset is defined in MutationSettings.
	 */

	virtual typename Population::Population<FITNESS_TYPE>::chromosome_container doChoose
		(typename Population::Population<FITNESS_TYPE>::chromosome_container chromosomeInputSet);
};

} //choosing
} //Op
} //GenLib



#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_CHOOSERANDOM_H_ */
