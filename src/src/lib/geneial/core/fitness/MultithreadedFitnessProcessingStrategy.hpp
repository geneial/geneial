#ifndef MULTITHREADEDFITNESSPROCESSINGSTRATEGY_HPP_
#define MULTITHREADEDFITNESSPROCESSINGSTRATEGY_HPP_

#include <geneial/core/fitness/MultithreadedFitnessProcessingStrategy.h>

namespace GeneticLibrary {

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Chromosome;


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
	_startBarrier.wait();
	_shutdown = true;
	_endBarrier.wait();

	for(unsigned int i = 0; i < _numWorkerThreads;i++)
	{
		_workerThreads[i]->join();
	}

}


template <typename FITNESS_TYPE>
void MultiThreadedFitnessProcessingStrategy<FITNESS_TYPE>::workerTask(unsigned int id)
{
	//Wait for any input to become available.
	while(true)
	{
		_startBarrier.wait();

		bool queueEmpty = false;
		std::queue<typename BaseChromosome<FITNESS_TYPE>::ptr >* myThreadq(_threadQueue[id]);
		while(!queueEmpty)
		{

			typename BaseChromosome<FITNESS_TYPE>::ptr chromosome;

			//Make sure only
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

		_endBarrier.wait();

		if(_shutdown) {
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


}  // namespace GeneticLibrary

#endif /* MULTITHREADEDFITNESSPROCESSINGSTRATEGY_HPP_ */
