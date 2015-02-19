/*
 * ChooseRandom.hpp
 *
 *  Created on: Jan 12, 2015
 *      Author: lukas
 */

#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_CHOOSERANDOM_HPP_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_CHOOSERANDOM_HPP_

#include <geneial/core/operations/choosing/ChooseRandom.h>
#include <geneial/utility/Random.h>


namespace GeneticLibrary {
namespace Operation {
namespace Choosing {

using namespace GeneticLibrary::Utility;

template <typename VALUE_TYPE, typename FITNESS_TYPE>
typename Population::Population<FITNESS_TYPE>::chromosome_container ChooseRandom<VALUE_TYPE,FITNESS_TYPE>::doChoose(
		typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container chromosomeInputSet){

	typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container::iterator chromosomeInputContainer_it;
	typename GeneticLibrary::Population::Population<FITNESS_TYPE>::chromosome_container chromosomeOutputSet;

	for (chromosomeInputContainer_it = chromosomeInputSet.begin();
				chromosomeInputContainer_it != chromosomeInputSet.end(); ++chromosomeInputContainer_it){

		//Uses Mutation Settings
		if (Random::instance()->decision(this->getSettings()->getPropability())){
			chromosomeOutputSet.push_back(*chromosomeInputContainer_it);
		}
	}
	return chromosomeOutputSet;
}

} //Choosing
} //Op
} //GenLib

#endif


