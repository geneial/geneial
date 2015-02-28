/*
 * MultiValueNPointCrossover.h
 *
 *  Created on: Dec 12, 2014
 *      Author: bewo
 */

#ifndef MULTIVALUE_CHROMOSOME_N_POINT_CROSSOVER_SETTINGS_H_
#define MULTIVALUE_CHROMOSOME_N_POINT_CROSSOVER_SETTINGS_H_

#include <cassert>

namespace GeneticLibrary {
namespace Operation {
namespace Crossover {

using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Operation::Coupling;

class MultiValueChromosomeNPointCrossoverSettings{

public:

	/**
	 * Every selection operation should allow the following modes:
	 * 	RANDOM_WIDTH -- Crossoverwidth are chosen at random
	 * 	RANDOM_MIN_WIDTH -- Crossoverwidth are chosen at random, but a min width has to be respected
	 * 	EQUIDISTANT_WIDTH -- Crossoverwith is equidistant
	 *
	 *
	 * Example:
	 * ========
	 *
	 * AAAAAAAAAAAAAAA
	 *
	 * BBBBBBBBBBBBBBB
	 *
	 * given a 2-Point-Crossover
	 *
	 * Equidistant width child
	 *
	 * width = 5
	 * BBBBB|AAAAA|BBBBB
	 *
	 * RANDOM_MIN_WIDTH, min. width = 2
	 * widths = {5,2,8}
	 *
	 * BBBBB|AA|BBBBBBBB
	 *
	 * RANDOM_WIDTH,
	 * widths = {2,2,13}
	 *
	 * BB|AA|BBBBBBBBBBB
	 *
	 */
	typedef enum { RANDOM_WIDTH, RANDOM_MIN_WIDTH, EQUIDISTANT_WIDTH } width_settings;

	MultiValueChromosomeNPointCrossoverSettings(unsigned int crossOverPoints, width_settings width_setting, unsigned int min_width = 1):
		_crossOverPoints(crossOverPoints),
		_width_setting(width_setting),
		_min_width(min_width){
		assert(crossOverPoints >0);
		assert(width_setting != RANDOM_MIN_WIDTH || min_width > 1);
	};

	virtual ~MultiValueChromosomeNPointCrossoverSettings() {
	};

	unsigned int getCrossOverPoints() const {
		return _crossOverPoints;
	};

	void setCrossOverPoints(unsigned int crossOverPoints) {
		_crossOverPoints = crossOverPoints;
	}
	unsigned int getMinWidth() const {
		return _min_width;
	}

	void setMinWidth(unsigned int minWidth) {
		_min_width = minWidth;
	}

	width_settings getWidthSetting() const {
		return _width_setting;
	}

	void setWidthSetting(width_settings widthSetting) {
		_width_setting = widthSetting;
	}

	;

private:
	unsigned int _crossOverPoints;
	width_settings _width_setting;
	unsigned int _min_width;


};

} /* namespace Crossover */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#endif /* MULTIVALUE_CHROMOSOME_N_POINT_CROSSOVER_SETTINGS_H_ */
