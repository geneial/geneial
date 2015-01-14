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

namespace GeneticLibrary {
namespace Operation {
namespace Choosing {

using namespace GeneticLibrary::Operation::Mutation;

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class ChooseRandom: public BaseChoosingOperation<FITNESS_TYPE> {
private:
	MutationSettings *_settings;

public:
	ChooseRandom(MutationSettings *settings) :
		BaseChoosingOperation<FITNESS_TYPE>(),
		_settings(settings) {
	}
	;
	virtual ~ChooseRandom() {
	}
	;

	/*
	 * Chooses from an chromosomeContainer Random values. The propability that a Chromosome is choosen and therefore part of the
	 * resultset is defined in MutationSettings.
	 */

	virtual typename Population::Population<FITNESS_TYPE>::chromosome_container doChoose(
			typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container chromosomeInputSet);

	const MutationSettings* getSettings() const {
		return _settings;
	}

	void setSettings(const MutationSettings*& settings) {
		_settings = settings;
	}
};

} //choosing
} //Op
} //GenLib

#include <geneial/core/operations/choosing/ChooseRandom.hpp>

#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_CHOOSERANDOM_H_ */
