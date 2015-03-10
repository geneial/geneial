#ifndef __GENEIAL_MULTI_VALUE_CHROMOSOME_FACTORY_H_
#define __GENEIAL_MULTI_VALUE_CHROMOSOME_FACTORY_H_

#include <geneial/core/population/builder/BaseChromosomeFactory.h>
#include <geneial/core/population/builder/MultiValueBuilderSettings.h>

namespace geneial {
namespace population {
namespace chromosome {


template <typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeFactory : public BaseChromosomeFactory<FITNESS_TYPE>{
protected:
	MultiValueBuilderSettings<VALUE_TYPE,FITNESS_TYPE> *_settings;
public:
	MultiValueChromosomeFactory(MultiValueBuilderSettings<VALUE_TYPE,FITNESS_TYPE> *settings):_settings(settings){};

	typename BaseChromosome<FITNESS_TYPE>::ptr createChromosome(typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues = BaseChromosomeFactory<FITNESS_TYPE>::CREATE_VALUES) = 0;

	const MultiValueBuilderSettings<VALUE_TYPE,FITNESS_TYPE>* getSettings() const
	{
		return _settings;
	}

	void setSettings(const MultiValueBuilderSettings<VALUE_TYPE,FITNESS_TYPE>*& settings)
	{
		_settings = settings;
	}
};

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

#endif /* __GENEIAL_MULTI_VALUE_CHROMOSOME_FACTORY_H_ */
