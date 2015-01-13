/*
 * ChooseRandom.hpp
 *
 *  Created on: Jan 12, 2015
 *      Author: lukas
 */

#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_CHOOSERANDOM_HPP_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_CHOOSERANDOM_HPP_

#include <geneial/core/operations/choosing/ChooseRandom.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>

namespace GeneticLibrary {
namespace Operation {
namespace Choosing {

template <typename VALUE_TYPE, typename FITNESS_TYPE>

typename BaseChoosingOperation<FITNESS_TYPE>::choosingChromosomeSet ChooseRandom<VALUE_TYPE, FITNESS_TYPE>::doChoose(
			typename BaseChoosingOperation<FITNESS_TYPE>::choosingChromosomeSet chromosomeInputSet
		){
	std:cout << "tadaa" << endl;

};

} //Choosing
} //Op
} //GenLib




