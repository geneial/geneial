#ifndef __GENEIAL_SINGLE_THREADED_FITNESS_PROCESSING_STRATEGY_H_
#define __GENEIAL_SINGLE_THREADED_FITNESS_PROCESSING_STRATEGY_H_

#include <geneial/core/population/Population.h>
#include <geneial/core/fitness/BaseFitnessProcessingStrategy.h>


namespace geneial {

using namespace geneial::population;

template <typename FITNESS_TYPE>
class SingleThreadedFitnessProcessingStrategy : public BaseFitnessProcessingStrategy<FITNESS_TYPE>
{
public:

	virtual void ensureHasFitness(const typename Population<FITNESS_TYPE>::chromosome_container &refcontainer);


	virtual ~SingleThreadedFitnessProcessingStrategy(){};
};


}  /* namespace geneial */

#include <geneial/core/fitness/SingleThreadedFitnessProcessingStrategy.hpp>


#endif /* __GENEIAL_SINGLE_THREADED_FITNESS_PROCESSING_STRATEGY_H_ */
