/*
 * Population.h
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include "utility/printable.h"
#include "population/chromosome/BasicChromosome.h"
#include <vector>

using namespace GeneticLibrary::Utility;

namespace GeneticLibrary {
namespace Population {


class Population : public printable {

private:
	std::vector<GeneticLibrary::Population::Chromosome::BasicChromosome> chromosomes;

public:
	Population();
	virtual ~Population();
	virtual void print(std::ostream& os) const;
	std::vector<GeneticLibrary::Population::Chromosome::BasicChromosome> getPopulation();
	void Population::generatePopulation();
};

} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* POPULATION_H_ */
