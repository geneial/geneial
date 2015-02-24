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

	static int i; //TODO (bewo) used shared mem?

	void workerTask()
	{
		const int id = MultiThreadedFitnessProcessingStrategy<FITNESS_TYPE>::i++;

		_queueMutex.lock();
		std::queue<typename BaseChromosome<FITNESS_TYPE>::ptr > *threadQ = _threadQueue[id];
		assert(threadQ);
		_queueMutex.unlock();

		bool queueEmpty = false;
		while(!queueEmpty){

			typename BaseChromosome<FITNESS_TYPE>::ptr chromosome;

			//Make sure only
			queueEmpty = threadQ->empty();
			if(!queueEmpty)
			{
				chromosome = threadQ->front();
				assert(chromosome);
				threadQ->pop();
			}

			if(chromosome)
			{
				//Force evaluation.
				chromosome->getFitness();
			}
		}
	}

	virtual void ensureHasFitness(const typename Population<FITNESS_TYPE>::chromosome_container &refcontainer)
	{
		assert(_threadQueue.empty());

		assert(_workerThreads.empty());

		assert(i==0);

		//Avoid zombies..
		unsigned int spawnThreads = _numWorkerThreads;
		size_t queueSize = refcontainer.size();
		if(queueSize < _numWorkerThreads)
		{
			spawnThreads = queueSize;
		}

		for(unsigned int i = 0; i < spawnThreads;i++)
		{
			std::queue<typename BaseChromosome<FITNESS_TYPE>::ptr>* ptr = new std::queue<typename BaseChromosome<FITNESS_TYPE>::ptr>();
			_threadQueue.push_back(ptr);
		}

		unsigned int j = 0;
		for(typename Population<FITNESS_TYPE>::chromosome_container::const_iterator it = refcontainer.begin();
				it != refcontainer.end();++it){
			if(!(*it)->hasFitness()){
				assert(_threadQueue[j%spawnThreads]);
				_threadQueue[j%spawnThreads]->push(*it);
				j++;
			}else{
				std::cout << "HAS FITNESS!" << std::endl;
			}
		}

		//Start Worker Threads...
		for(unsigned int i = 0; i < spawnThreads;i++)
		{
			boost::thread*  workerThread = new boost::thread(
					boost::bind(&MultiThreadedFitnessProcessingStrategy::workerTask, this)
			);
			_workerThreads.push_back(workerThread);
		}


		//Wait for workers to shutdown...
		for(std::vector<boost::thread*>::iterator tid = _workerThreads.begin();tid != _workerThreads.end();tid++)
		{
			(*tid)->join();
		}

		for(unsigned int i = 0; i < spawnThreads;i++)
		{
			assert(_threadQueue[i]);
			delete _threadQueue[i];
		}



		//Cleanup.
		_workerThreads.clear();
		_threadQueue.clear();

		i = 0;
		assert(_threadQueue.empty());
	};

	virtual ~MultiThreadedFitnessProcessingStrategy() {
	}

	unsigned int getNumWorkerThreads() const {
		return _numWorkerThreads;
	}

	;
private:
	unsigned int _numWorkerThreads;

	boost::mutex _queueMutex;

	std::vector<boost::thread*> _workerThreads;

	std::vector< std::queue<typename BaseChromosome<FITNESS_TYPE>::ptr >*> _threadQueue;


};

template <typename FITNESS_TYPE>
int GeneticLibrary::MultiThreadedFitnessProcessingStrategy<FITNESS_TYPE>::i = 0;


}  // namespace GeneticLibrary

#endif /* MULTITHREADEDFITNESSPROCESSINGSTRATEGY_H_ */
