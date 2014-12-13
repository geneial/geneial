/*
 * MultiValueNPointCrossover.h
 *
 *  Created on: Dec 12, 2014
 *      Author: bewo
 */

#ifndef MULTIVALUE_CHROMOSOME_N_POINT_CROSSOVER_SETTINGS_H_
#define MULTIVALUE_CHROMOSOME_N_POINT_CROSSOVER_SETTINGS_H_

#include "assert.h"

namespace GeneticLibrary {
namespace Operation {
namespace Crossover {

using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Operation::Coupling;

class MultiValueChromosomeNPointCrossoverSettings{
public:
	MultiValueChromosomeNPointCrossoverSettings(unsigned int crossOverPoints): _crossOverPoints(crossOverPoints) {
		assert(crossOverPoints >0);
	};
	virtual ~MultiValueChromosomeNPointCrossoverSettings() {
	}

	unsigned int getCrossOverPoints() const {
		return _crossOverPoints;
	}

	void setCrossOverPoints(unsigned int crossOverPoints) {
		_crossOverPoints = crossOverPoints;
	}

	;
private:
	unsigned int _crossOverPoints;
};

} /* namespace Crossover */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#endif /* MULTIVALUE_CHROMOSOME_N_POINT_CROSSOVER_SETTINGS_H_ */
