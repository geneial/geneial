/*
 * BaseFitnessProcessingStrategy.h
 *
 *  Created on: Feb 23, 2015
 *      Author: bewo
 */

#ifndef BASEFITNESSPROCESSINGSTRATEGY_H_
#define BASEFITNESSPROCESSINGSTRATEGY_H_

#include <geneial/core/population/Population.h>
#include <geneial/core/population/ContainerTypes.h>


namespace GeneticLibrary {

using namespace GeneticLibrary::Population;

template <typename FITNESS_TYPE>
class BaseFitnessProcessingStrategy
{
public:
	virtual void ensureHasFitness(const typename ContainerTypes<FITNESS_TYPE>::chromosome_container &refcontainer) = 0;
	virtual ~BaseFitnessProcessingStrategy(){};
};

}  // namespace GeneticLibrary

#endif /* BASEFITNESSPROCESSINGSTRATEGY_H_ */
