#ifndef __GENEIAL_MULTI_INT_VALUE_CHROMOSOME_FACTORY_H_
#define __GENEIAL_MULTI_INT_VALUE_CHROMOSOME_FACTORY_H_

#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>

namespace geneial {
namespace population {
namespace chromosome {

template <typename FITNESS_TYPE>
class MultiIntValueChromosomeFactory : public MultiValueChromosomeFactory<int,FITNESS_TYPE>
{

public:
	MultiIntValueChromosomeFactory(MultiValueBuilderSettings<int,FITNESS_TYPE> *settings): MultiValueChromosomeFactory<int,FITNESS_TYPE>(settings) {};
	typename BaseChromosome<FITNESS_TYPE>::ptr createChromosome(bool populateValues = true);
};

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

#include <geneial/core/population/builder/MultiIntValueChromosomeFactory.hpp>

#endif /* __GENEIAL_MULTI_INT_VALUE_CHROMOSOME_FACTORY_H_ */
