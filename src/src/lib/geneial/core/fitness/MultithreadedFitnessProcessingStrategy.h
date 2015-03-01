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
		_numWorkerThreads(numWorkerThreads)
		{
		assert(_numWorkerThreads > 0);
		startWorkers();
	}


	void startWorkers(){
		_shutdown = false;
		_started = false;
		for(unsigned int i = 0; i < _numWorkerThreads;i++)
		{
			boost::thread*  workerThread = new boost::thread(
					boost::bind(&MultiThreadedFitnessProcessingStrategy::workerTask, this,i)
			);
			_threadQueue.push_back(new std::queue<typename BaseChromosome<FITNESS_TYPE>::ptr>());
			_workerThreads.push_back(workerThread);
		}
	}

	void stopWorkers(){
	    _startBarrier.wait();
	    _shutdown = true;
	    _endBarrier.wait();

		for(unsigned int i = 0; i < _numWorkerThreads;i++)
		{
			_workerThreads[i]->join();
		}

	}



	void workerTask(unsigned int id)
	{
		//Wait for any input to become available.
		while(true){

			_startBarrier.wait();

			bool queueEmpty = false;
			std::queue<typename BaseChromosome<FITNESS_TYPE>::ptr >* myThreadq(_threadQueue[id]);
			while(!queueEmpty){

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

	virtual void ensureHasFitness(const typename Population<FITNESS_TYPE>::chromosome_container &refcontainer)
	{

		unsigned int j = 0;
		for(typename Population<FITNESS_TYPE>::chromosome_container::const_iterator it = refcontainer.begin();
				it != refcontainer.end();++it){
			if(!(*it)->hasFitness()){
				assert(*it);
				_threadQueue[j%_numWorkerThreads]->push(*it);
				j++;
			}else{
				std::cout << "HAS FITNESS!" << std::endl;
			}
		}

		//Start Signal!
		_startBarrier.wait();

	    //Wait for workers to be complete
	    _endBarrier.wait();

	};

	virtual ~MultiThreadedFitnessProcessingStrategy() {
		stopWorkers();
	}

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

#endif /* MULTITHREADEDFITNESSPROCESSINGSTRATEGY_H_ */
