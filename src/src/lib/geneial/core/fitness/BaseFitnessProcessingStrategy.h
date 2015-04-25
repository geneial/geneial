#ifndef __GENEIAL_BASE_FITNESS_PROCESSING_STRATEGY_H_
#define __GENEIAL_BASE_FITNESS_PROCESSING_STRATEGY_H_

#include <geneial/core/population/Population.h>
#include <geneial/core/population/ContainerTypes.h>

namespace geneial
{

using namespace geneial::population;

template<typename FITNESS_TYPE>
class BaseFitnessProcessingStrategy
{
public:

    virtual void ensureHasFitness(const typename ContainerTypes<FITNESS_TYPE>::chromosome_container &refcontainer) = 0;

    virtual ~BaseFitnessProcessingStrategy()
    {
    }
};

} /*namespace geneial*/

#endif /* __GENEIAL_BASE_FITNESS_PROCESSING_STRATEGY_H_ */
