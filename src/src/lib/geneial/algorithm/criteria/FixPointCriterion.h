#pragma once

#include <geneial/algorithm/criteria/StatefulStoppingCriterion.h>

#include <deque>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
geneial_private_namespace(stopping_criteria)
{
using ::geneial::population::management::BaseManager;

geneial_export_namespace
{

//TODO (bewo) proper doc
//Idea to implemented here
//count the last best xx chromosomes, each iteration count the derivation in the best chromosome
//abort if derivation is below certain threshold (typically 0)
template <typename FITNESS_TYPE>
class FixPointCriterion : public StatefulStoppingCriterion<FITNESS_TYPE>
{
public:
    virtual ~FixPointCriterion() {};

    FixPointCriterion(const FITNESS_TYPE threshold = 0, const FITNESS_TYPE windowSize = 10, const FITNESS_TYPE minDiff = 1):
                     _windowSize(windowSize),
                     _threshold(threshold),
                     _minDiff(minDiff)
    {
        assert(_minDiff <= _windowSize);
        assert(_minDiff > 0);
    }

    virtual void print(std::ostream& os) const
    {
        os << "FixPoint"
                << "(thres:"
                << _threshold
                << ",min:"
                << _minDiff
                << "winsize:"
                << _windowSize
                << ")";
    }

    virtual bool wasStatefullyReached(BaseManager<FITNESS_TYPE> &manager)
    {
        updateWindowValues(manager);

        bool result = false;

        if(_window.size() >= _minDiff)
        {
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
            const FITNESS_TYPE bestFitness = manager.getHighestFitness();
            assert(_window.size() <= _windowSize);

            _window.push_front(bestFitness);

            while(_window.size()>_windowSize)
            {
                _window.pop_back();
            }

            assert(_window.size() <= _windowSize);
    }

private:
    //Window to "look back"
    const unsigned int _windowSize;

    //Difference threshold between first and last of window
    FITNESS_TYPE _threshold;

    std::deque<FITNESS_TYPE> _window;

    //minimum window size for a compare
    const unsigned int _minDiff;

};

} /* geneial_export_namespace */
} /* private namespace stopping_criteria */
} /* private namespace algorithm */
} /* private namespace geneial */
