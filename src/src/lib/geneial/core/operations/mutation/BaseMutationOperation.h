/*
 * BaseMutationOperation.h
 *
 *  Created on: Dec 19, 2014
 *      Author: lukas
 */
#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_BASEMUTATIONOPERATION_H_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_BASEMUTATIONOPERATION_H_

#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/core/population/Population.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>


namespace GeneticLibrary {
namespace Operation {
namespace Mutation {

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Operation::Choosing;

template<typename FITNESS_TYPE>
class BaseMutationOperation {
private:
	MutationSettings* _settings;
	BaseChoosingOperation<FITNESS_TYPE>* _choosingOperation;
public:
	BaseMutationOperation(MutationSettings *settings, BaseChoosingOperation<FITNESS_TYPE> *choosingOperation) :
			_settings(settings),
			_choosingOperation(choosingOperation)
			{
	}
	; //Constructor
	virtual ~BaseMutationOperation() {
	}
	; //Destructor
	typedef typename Population<FITNESS_TYPE>::chromosome_container mutation_result_set;
	virtual typename Population<FITNESS_TYPE>::chromosome_container doMutate(
			typename Population<FITNESS_TYPE>::chromosome_container mutants,
			BaseManager<FITNESS_TYPE> &manager
			) =0;
	MutationSettings* const & getSettings() const {
		return _settings;
	}
	;
	void setSettings(const MutationSettings*& settings) {
		_settings = settings;
	}

	BaseChoosingOperation<FITNESS_TYPE>* getChoosingOperation() const {
		return _choosingOperation;
	}

	;
};
//class
}//namespace Mutation
} //namespace Operation
} //namespace GeneticLibrary
#endif //SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_BASEMUTATIONOPERATION_H_
