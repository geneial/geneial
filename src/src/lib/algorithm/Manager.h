/*
 * Manager.h
 *
 *  Created on: Dec 8, 2014
 *      Author: lukas
 */

#ifndef SRC_LIB_ALGORITHM_MANAGER_H_
#define SRC_LIB_ALGORITHM_MANAGER_H_

namespace GeneticLibrary {
namespace Manager {

class Manager{
private:

public:

	GeneticLibrary::Population::Chromosome::BasicChromosome selectBestChromosome(GeneticLibrary::Population::Population population);

	GeneticLibrary::Population::Population replenishPopulation (GeneticLibrary::Population::Population population);

};


} /* namespace Manager */
} /* namespace GeneticLibrary */




#endif /* SRC_LIB_ALGORITHM_MANAGER_H_ */
