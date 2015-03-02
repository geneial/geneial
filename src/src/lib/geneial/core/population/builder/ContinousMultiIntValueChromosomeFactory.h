#ifndef CONT_MULTI_INT_VALUE_CHROMOSOME_FACTORY_H_
#define CONT_MULTI_INT_VALUE_CHROMOSOME_FACTORY_H_

#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/core/population/builder/MultiIntValueChromosomeFactory.h>
#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

template <typename FITNESS_TYPE>
class ContinousMultiIntValueChromosomeFactory : public MultiIntValueChromosomeFactory<FITNESS_TYPE>
{
private:
	ContinousMultiValueBuilderSettings<int,FITNESS_TYPE> *_settings;
public:
	ContinousMultiIntValueChromosomeFactory(ContinousMultiValueBuilderSettings<int,FITNESS_TYPE> *settings): MultiIntValueChromosomeFactory<FITNESS_TYPE>(settings),
	_settings(settings){};
	typename BaseChromosome<FITNESS_TYPE>::ptr createChromosome(bool populateValues = true);
};

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#include <geneial/core/population/builder/ContinousMultiIntValueChromosomeFactory.hpp>

#endif /* CONT_MULTI_INT_VALUE_CHROMOSOME_FACTORY_H_ */
