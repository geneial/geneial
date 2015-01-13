/*
 * BaseChoosingOperation.h
 *
 *  Created on: Jan 12, 2015
 *      Author: lukas
 */

#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_BASECHOOSINGOPERATION_H_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_BASECHOOSINGOPERATION_H_

#include <geneial/core/operations/mutation/MutationSettings.h>

namespace GeneticLibrary {
namespace Operation {
namespace Choosing {

using namespace GeneticLibrary::Operation::Mutation;

template <typename FITNESS_TYPE>
class BaseChoosingOperation{
private:
	MutationSettings* _settings;
public:
	BaseChoosingOperation (MutationSettings *settings): _settings(settings) {};
	virtual ~BaseChoosingOperation() {};
	typedef typename Population::Population<FITNESS_TYPE>::chromosome_container chosingChromosomeSet;

	virtual chosingChromosomeSet doChoose (typename Choosing::BaseChoosingOperation<FITNESS_TYPE>::chosingChromosomeSet chromosomeInputSet)=0;
	MutationSettings* const& getSettings() const {
		return _settings;
	}
};

} //namespace Mutation
} //namespace Operation
} //namespace GenLib



#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_BASECHOOSINGOPERATION_H_ */
