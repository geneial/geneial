/*
 * BaseChromosomeFactory.h
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#ifndef BASECHROMOSOMEFACTORY_H_
#define BASECHROMOSOMEFACTORY_H_

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

template <typename FITNESS_TYPE>
class BaseChromosomeFactory {
private:
public:
	BaseChromosomeFactory() {};
	virtual ~BaseChromosomeFactory() {}
	virtual typename BaseChromosome<FITNESS_TYPE>::ptr createChromosome(bool populateValues = true) = 0;
};

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* BASECHROMOSOMEFACTORY_H_ */
