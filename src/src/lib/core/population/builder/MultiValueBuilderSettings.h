/*
 * BuilderSettings.h
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#ifndef MULTI_VALUE_BUILDER_SETTING_H_
#define MULTI_VALUE_BUILDER_SETTING_H_

#include <core/population/builder/BuilderSettings.h>

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

template <typename VALUE_TYPE>
class MultiValueBuilderSettings : public BuilderSettings<VALUE_TYPE> {
private:
	unsigned int _num;

public:
	MultiValueBuilderSettings() : BuilderSettings<VALUE_TYPE>(),_num() {};
	MultiValueBuilderSettings(unsigned int num, VALUE_TYPE random_max,	VALUE_TYPE random_min ): BuilderSettings<VALUE_TYPE>(random_max,random_min), _num(num) {};
	virtual ~MultiValueBuilderSettings() {};

	unsigned int getNum() const {
		return _num;
	}

	void setNum(unsigned int num) {
		_num = num;
	}
};

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* MULTI_VALUE_BUILDER_SETTING_H_ */
