#ifndef BESTCHROMOSOMEOBSERVER_H_
#define BESTCHROMOSOMEOBSERVER_H_

#include <geneial/algorithm/observer/AlgorithmObserver.h>
#include <set>

namespace GeneticLibrary {
namespace Algorithm {

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Operation;

template <typename FITNESS_TYPE>
class BestChromosomeObserver : public AlgorithmObserver<FITNESS_TYPE> {
public:
	BestChromosomeObserver():
		_foundBest(false)
	{
		this->addSubscribedEvent(AlgorithmObserver<FITNESS_TYPE>::GENERATION_DONE);
	}
	virtual ~BestChromosomeObserver(){};

	virtual void updateGeneration(BaseManager<FITNESS_TYPE> &manager)
	{
		const FITNESS_TYPE lastBest = manager.getHighestFitness();
		if(lastBest > _best || ! _foundBest)
		{
			_best  = lastBest;
			_foundBest = true;
			updateNewBestChromosome(manager);

		}
	}

	virtual void updateNewBestChromosome(BaseManager<FITNESS_TYPE> &manager){}
private:
	FITNESS_TYPE _best;
	bool _foundBest;
};

} //Algorithm
} //GeneticLibrary

#endif /* BESTCHROMOSOMEOBSERVER_H_ */
