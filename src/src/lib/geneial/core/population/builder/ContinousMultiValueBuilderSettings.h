#ifndef __GENEIAL_CONTINOUS_MULTI_VALUE_BUILDER_SETTING_H_
#define __GENEIAL_CONTINOUS_MULTI_VALUE_BUILDER_SETTING_H_

#include <geneial/core/population/builder/MultiValueBuilderSettings.h>

namespace geneial {
namespace population {
namespace chromosome {

template <typename VALUE_TYPE, typename FITNESS_TYPE>
class ContinousMultiValueBuilderSettings : public MultiValueBuilderSettings<VALUE_TYPE,FITNESS_TYPE> {
private:
	bool _hasStart; //Do we have a starting point?
	VALUE_TYPE _start; //the starting point from which to start the chromosome array
	VALUE_TYPE _eps; //eps delta deriation between values in the array


public:
	ContinousMultiValueBuilderSettings(const typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator, unsigned int num, VALUE_TYPE random_max, VALUE_TYPE random_min, bool hasStart = false, VALUE_TYPE start = 0, VALUE_TYPE eps = 1):
		MultiValueBuilderSettings<VALUE_TYPE,FITNESS_TYPE>(fitnessEvaluator,num,random_max,random_min),
		_hasStart(hasStart),
		_start(start), //Start point for generating ContinousChromomsomes
		_eps(eps) //Eps derivation from previous point per step
		{}

	virtual ~ContinousMultiValueBuilderSettings() {}

	VALUE_TYPE getEps() const
	{
		return _eps;
	}

	VALUE_TYPE getStart() const
	{
		return _start;
	}

	bool hasStart() const
	{
		return _hasStart;
	}

};

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

#endif /* __GENEIAL_CONTINOUS_MULTI_VALUE_BUILDER_SETTING_H_ */
