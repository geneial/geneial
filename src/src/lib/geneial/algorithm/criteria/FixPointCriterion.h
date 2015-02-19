/*
 * FitnessDeltaCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef FITNESSDELTACRITERION_H_
#define FITNESSDELTACRITERION_H_

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/StatefulStoppingCriterion.h>
#include <deque>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

//Idea to implemented here
//count the last best xx chromosomes, each iteration count the derivation in the best chromosome
//abort if derivation is below certain threshold (typically 0)
template <typename FITNESS_TYPE>
class FixPointCriterion : public StatefulStoppingCriterion<FITNESS_TYPE> {
public:
	virtual ~FixPointCriterion() {};

	virtual FixPointCriterion(FITNESS_VALUE threshold = 0, windowSize = 10, minDiff = 1):
					_threshold(FITNESS_VALUE),
					_windowSize(windowSize), _minDiff(minDiff), _lastGeneration(0), StatefulStoppingCriterion() {
		assert(_minDiff <= _windowSize);
		assert(_minDiff > 0);
	};

	virtual void print(std::ostream& os) const
	{
		os << "FixPoint (thres:" << _threshold << ",min:" << _minDiff
				<< "winsize:" << _windowSize << ")";
	}

	virtual bool wasStatefullyReached(BaseManager<FITNESS_TYPE> &manager)
	{
		updateWindowValues(manager);

		boolean result = false;

		if(_window.size() >= _minDiff){
			const FITNESS_TYPE latest = _window.front();
			const FITNESS_TYPE oldest = _window.back();
			//Ignore if we became worse.
			if(latest >= oldest)
			{
				result = (latest - oldest) <= _threshold;
			}
		}

		return result;
	}

protected:
	void inline updateWindowValues(BaseManager<FITNESS_TYPE> &manager)
	{
			const FITNESS_TYPE bestFitness = manager.getPopulation().getHighestFitnessChromosome();
			const size_t currentWindowSize = _window.size();

			assert(currentWindowSize <= _windowSize);

			_window.push_front(bestFitness);

			while(_window.size()>_windowSize)
			{
				_window.pop_back();
			}

			const size_t afterInsertSize = _window.size();
			assert(afterInsertSize <= _windowSize);
	}

private:
	//Window to "look back"
	const unsigned int _windowSize;

	//Difference threshold between first and last of window
	FITNESS_VALUE _threshold;

	std::deque<typename FITNESS_VALUE> _window;

	//minimum window size for a compare
	const unsigned int _minDiff;

};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* FITNESSDELTACRITERION_H_ */
