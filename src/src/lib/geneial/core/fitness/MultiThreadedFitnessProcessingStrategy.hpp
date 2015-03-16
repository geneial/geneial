#ifndef __GENEIAL_MULTITHREADED_FITNESS_PROCESSING_STRATEGY_HPP_
#define __GENEIAL_MULTITHREADED_FITNESS_PROCESSING_STRATEGY_HPP_

#include <geneial/core/fitness/MultiThreadedFitnessProcessingStrategy.h>

namespace geneial {


template <typename FITNESS_TYPE>
void MultiThreadedFitnessProcessingStrategy<FITNESS_TYPE>::startWorkers()
{
	_shutdown = false;
	_started = true;

	for(unsigned int i = 0; i < _numWorkerThreads;i++)
	{
		boost::thread*  workerThread = new boost::thread(
				boost::bind(&MultiThreadedFitnessProcessingStrategy::workerTask, this,i)
		);
		_threadQueue.push_back(new std::queue<typename BaseChromosome<FITNESS_TYPE>::ptr>());
		_workerThreads.push_back(workerThread);
	}
}


template <typename FITNESS_TYPE>
void MultiThreadedFitnessProcessingStrategy<FITNESS_TYPE>::stopWorkers()
{
	if(_started){
		_startBarrier.wait();
		_shutdown = true;
		_endBarrier.wait();

		for(unsigned int i = 0; i < _numWorkerThreads;i++)
		{
			_workerThreads[i]->join();
		}
	}
}


template <typename FITNESS_TYPE>
void MultiThreadedFitnessProcessingStrategy<FITNESS_TYPE>::workerTask(unsigned int id)
{
	//TODO (bewo): From benchmarks it seems this code still has a cache-line / false-sharing problem
	//see https://www.youtube.com/watch?v=WDIkqP4JbkE

	//Wait until all worker threads have started.
	while(true)
	{
		//Wait for any input to become available.
		_startBarrier.wait();

		bool queueEmpty = false;
		std::queue<typename BaseChromosome<FITNESS_TYPE>::ptr >* myThreadq(_threadQueue[id]);

		while(!queueEmpty)
		{

			typename BaseChromosome<FITNESS_TYPE>::ptr chromosome;

			//Make sure queue is not empty,
			//Caution: this is necessary if start barrier was triggered without queue input (e.g., shutdown) , which can happen.
			//Do not try to be smart and refactor this without knowing what you are doing!
			queueEmpty = myThreadq->empty();


			if(!queueEmpty)
			{
				chromosome = myThreadq->front();
				assert(chromosome);
				myThreadq->pop();
			}

			if(chromosome)
			{
				//Force evaluation.
				chromosome->getFitness();
			}
		}

		//Wait until all worker threads have synchronized.
		_endBarrier.wait();

		if(_shutdown)
		{
			return;
		}
	}
}


template <typename FITNESS_TYPE>
void MultiThreadedFitnessProcessingStrategy<FITNESS_TYPE>::ensureHasFitness(const typename Population<FITNESS_TYPE>::chromosome_container &refcontainer)
{

	if(!_started)
	{
		startWorkers();
	}

	unsigned int j = 0;
	for(typename Population<FITNESS_TYPE>::chromosome_container::const_iterator it = refcontainer.begin();
			it != refcontainer.end();++it)
	{
		if(!(*it)->hasFitness())
		{
			assert(*it);
			_threadQueue[j%_numWorkerThreads]->push(*it);
			j++;
		}
	}

	//Start Signal!
	_startBarrier.wait();

	//Wait for workers to be complete
	_endBarrier.wait();

}


}  /* namespace geneial */

#endif /* __GENEIAL_MULTITHREADED_FITNESS_PROCESSING_STRATEGY_HPP_ */
