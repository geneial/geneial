#ifndef MULTITHREADEDFITNESSPROCESSINGSTRATEGY_H_
#define MULTITHREADEDFITNESSPROCESSINGSTRATEGY_H_

#include <geneial/core/population/Population.h>
#include <geneial/core/fitness/BaseFitnessProcessingStrategy.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>

#include <boost/thread.hpp>

#include <vector>
#include <queue>
#include <cassert>


namespace GeneticLibrary {

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Chromosome;

template <typename FITNESS_TYPE>
class MultiThreadedFitnessProcessingStrategy : public BaseFitnessProcessingStrategy<FITNESS_TYPE>
{
public:
	MultiThreadedFitnessProcessingStrategy(unsigned int numWorkerThreads):
		_startBarrier(numWorkerThreads + 1),
		_endBarrier(numWorkerThreads + 1),
		_started(false),
		_shutdown(false),
		_numWorkerThreads(numWorkerThreads)
	{
		assert(_numWorkerThreads > 0);
	}


	virtual ~MultiThreadedFitnessProcessingStrategy()
	{
		stopWorkers();
	}


	void startWorkers();

	void stopWorkers();

	void workerTask(unsigned int id);

	virtual void ensureHasFitness(const typename Population<FITNESS_TYPE>::chromosome_container &refcontainer);


	unsigned int getNumWorkerThreads() const {
		return _numWorkerThreads;
	}

	bool isStarted() const {
		return _started;
	}

	;
private:

	boost::barrier _startBarrier;
	boost::barrier _endBarrier;

	bool _started;
	bool _shutdown;

	unsigned int _numWorkerThreads;

	std::vector<boost::thread*> _workerThreads;

	std::vector< std::queue<typename BaseChromosome<FITNESS_TYPE>::ptr >* > _threadQueue;


};

}  // namespace GeneticLibrary

#include <geneial/core/fitness/MultithreadedFitnessProcessingStrategy.hpp>

#endif /* MULTITHREADEDFITNESSPROCESSINGSTRATEGY_H_ */
