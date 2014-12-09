/*
 * BaseGeneticAlgorithm.h
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef BASEGENETICALGORITHM_H_
#define BASEGENETICALGORITHM_H_



#include <core/population/management/BaseManager.h>

namespace GeneticLibrary {
namespace Algorithm {

class BaseGeneticAlgorithm {
public:
	BaseGeneticAlgorithm();
	virtual ~BaseGeneticAlgorithm();
};

} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* BASEGENETICALGORITHM_H_ */
