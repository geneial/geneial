/*
 * BaseCouplingOperation.h
 *
 *  Created on: Dec 12, 2014
 *      Author: bewo
 */

#ifndef BASECROSSOVEROPERATION_H_
#define BASECROSSOVEROPERATION_H_

namespace GeneticLibrary {
namespace Operation {
namespace Crossover {

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Operation::Coupling;

template <typename FITNESS_TYPE>
class BaseCrossoverOperation {
public:
	typedef typename Population<FITNESS_TYPE>::chromosome_container crossover_result_set;
	BaseCrossoverOperation() {};
	virtual ~BaseCrossoverOperation() {};
	virtual crossover_result_set
		doCrossover( typename BaseChromosome<FITNESS_TYPE>::ptr mommy, typename BaseChromosome<FITNESS_TYPE>::ptr daddy ) = 0;

	//Is the order of mommy for and daddy for doCrossover() relevant?, true if yes.
	virtual bool inline isSymmetric() const = 0;

};

} /* namespace Crossover */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#endif /* BASECROSSOVEROPERATION_H_ */
