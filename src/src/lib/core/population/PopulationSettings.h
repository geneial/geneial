/*
 * PopulationSettings.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef POPULATIONSETTINGS_H_
#define POPULATIONSETTINGS_H_


namespace GeneticLibrary {
namespace Population {

class PopulationSettings {
private:
	int _max_chromosomes;

public:
	PopulationSettings(int max_chromosomes) :_max_chromosomes(max_chromosomes){};
	virtual ~PopulationSettings() {};

	int getMaxChromosomes() const {
		return _max_chromosomes;
	}

	void setMaxChromosomes(int maxChromosomes) {
		_max_chromosomes = maxChromosomes;
	}
};

} /* namespace Population */
} /* namespace GeneticLibrary */


#endif /* POPULATIONSETTINGS_H_ */
