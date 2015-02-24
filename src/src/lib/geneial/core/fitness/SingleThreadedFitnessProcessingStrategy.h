#ifndef SINGLETHREADEDFITNESSPROCESSINGSTRATEGY_H_
#define SINGLETHREADEDFITNESSPROCESSINGSTRATEGY_H_

#include <geneial/core/population/Population.h>
#include <geneial/core/fitness/BaseFitnessProcessingStrategy.h>


namespace GeneticLibrary {

using namespace GeneticLibrary::Population;

template <typename FITNESS_TYPE>
class SingleThreadedFitnessProcessingStrategy : public BaseFitnessProcessingStrategy<FITNESS_TYPE>
{
public:
	virtual void ensureHasFitness(const typename Population<FITNESS_TYPE>::chromosome_container &refcontainer){
		for(typename Population<FITNESS_TYPE>::chromosome_container::iterator it = refcontainer.begin(); it != refconainer.end();++it){
			it->getFitness(); //Will trigger lazy evaluation.
		}
	};
	virtual ~SingleThreadedFitnessProcessingStrategy(){};
};


}  // namespace GeneticLibrary

#endif /* SINGLETHREADEDFITNESSPROCESSINGSTRATEGY_H_ */
