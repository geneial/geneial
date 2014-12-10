/*
 * BuilderSettings.h
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#ifndef BUILDERSETTINGS_H_
#define BUILDERSETTINGS_H_

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

template <typename VALUE_TYPE>
class BuilderSettings {
private:
	VALUE_TYPE _random_max;
	VALUE_TYPE _random_min;

public:
	BuilderSettings() {};
	BuilderSettings(VALUE_TYPE random_max,	VALUE_TYPE random_min) : _random_max(random_max),_random_min(random_min) {};
	virtual ~BuilderSettings() {};

	VALUE_TYPE getRandomMax() const {
		return _random_max;
	}

	void setRandomMax(VALUE_TYPE randomMax) {
		_random_max = randomMax;
	}

	VALUE_TYPE getRandomMin() const {
		return _random_min;
	}

	void setRandomMin(VALUE_TYPE randomMin) {
		_random_min = randomMin;
	}
};

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* BUILDERSETTINGS_H_ */
