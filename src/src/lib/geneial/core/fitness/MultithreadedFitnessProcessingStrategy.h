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
	MultiThreadedFitnessProcessingStrategy(unsigned int numWorkerThreads):_numWorkerThreads(numWorkerThreads){
		assert(_numWorkerThreads > 0);
	}

	void workerTask(){
		bool queueEmpty = false;
		while(!queueEmpty){

			typename BaseChromosome<FITNESS_TYPE>::ptr chromosome;

			//Make sure only
			_queueMutex.lock();
			{
				queueEmpty = _queue.empty();
				if(!queueEmpty)
				{
					chromosome = _queue.front();
					_queue.pop();
				}
			}
			_queueMutex.unlock();

			//Release Mutex
			if(chromosome)
			{
				//Force evaluation.
				chromosome->getFitness();
			}
		}
	}

	virtual void ensureHasFitness(const typename Population<FITNESS_TYPE>::chromosome_container &refcontainer){
		assert(_queue.empty());

		assert(_workerThreads.empty());

		for(typename Population<FITNESS_TYPE>::chromosome_container::const_iterator it = refcontainer.begin();
				it != refcontainer.end();++it){
			_queue.push(*it);
		}

		//Avoid zombies..
		unsigned int spawnThreads = _numWorkerThreads;
		size_t queueSize = _queue.size();
		if(queueSize < _numWorkerThreads)
		{
			spawnThreads = queueSize;
		}


		//Start Worker Threads...
		for(unsigned int i = 0; i < spawnThreads;i++)
		{
			boost::thread*  workerThread = new boost::thread(
					boost::bind(&MultiThreadedFitnessProcessingStrategy::workerTask, this)
			);
			_workerThreads.push_back(workerThread);
		}


		//Wait for workers to shutdown
		for(std::vector<boost::thread*>::iterator tid = _workerThreads.begin();tid != _workerThreads.end();tid++)
		{
			(*tid)->join();
		}

		//Cleanup.
		_workerThreads.clear();

		assert(_queue.empty());
	};

	virtual ~MultiThreadedFitnessProcessingStrategy() {
	}

	unsigned int getNumWorkerThreads() const {
		return _numWorkerThreads;
	}

	;
private:
	unsigned int _numWorkerThreads;

	std::vector<boost::thread*> _workerThreads;

	boost::mutex _queueMutex;

	std::queue<typename BaseChromosome<FITNESS_TYPE>::ptr > _queue;


};


}  // namespace GeneticLibrary

#endif /* MULTITHREADEDFITNESSPROCESSINGSTRATEGY_H_ */
