#ifndef __GENEIAL_CONSECUTIVE_CRITERION_H_
#define __GENEIAL_CONSECUTIVE_CRITERION_H_

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/StatefulStoppingCriterion.h>
#include <deque>

namespace geneial
{
namespace algorithm
{
namespace stopping_criteria
{

using namespace geneial::population::management;

/**
 * Decorator that checks whether a given criterion has been reached several times.
 */
template<typename FITNESS_TYPE>
class ConsecutiveDecorator: public StatefulStoppingCriterion<FITNESS_TYPE>
{

public:
    typedef typename BaseStoppingCriterion<FITNESS_TYPE>::ptr criterion;

    ConsecutiveDecorator(unsigned int windowSize, unsigned int consecutiveHits,
            typename BaseStoppingCriterion<FITNESS_TYPE>::ptr criterion) :
            _windowSize(windowSize), _consecutiveHits(consecutiveHits), _criterion(criterion)
    {
        assert(windowSize >= 1); //Allow wrapper valuss of 1 here, make it as flexible as possible
        assert(consecutiveHits > 0);
        assert(windowSize >= consecutiveHits);
    }

    virtual ~ConsecutiveDecorator()
    {
    }

    virtual bool wasStatefullyReached(BaseManager<FITNESS_TYPE> &manager)
    {
        updateWindowValues(manager);

        bool result = false;

        std::deque<bool>::iterator it = _window.begin();
        //run over deque, count how often the criteria was met.
        unsigned int count = 0;
        while (it != _window.end())
        {
            if (*it++)
            {
                count++;
            }
        }

        if (count >= _consecutiveHits)
        {
            result = true;
        }

        return result;
    }

protected:
    void inline updateWindowValues(BaseManager<FITNESS_TYPE> &manager)
    {
        assert(_window.size() <= _windowSize);
        _window.push_front(_criterion->wasReached(manager));

        while (_window.size() > _windowSize)
        {
            _window.pop_back();
        }
        assert(_window.size() <= _windowSize);
    }

    virtual void print(std::ostream& os) const
    {
        os << "Consecutive (criterion:" << _criterion << ", win: " << _windowSize << ")";
    }

    const criterion& getCriterion() const
    {
        return _criterion;
    }

    void setCriterion(const criterion& criterion)
    {
        _criterion = criterion;
    }

private:
    criterion _criterion;
    unsigned int _windowSize;
    unsigned int _consecutiveHits;
    std::deque<bool> _window;

};

} /* namespace stopping_criteria */
} /* namespace algorithm */
} /* namespace geneial */

#endif /* __GENEIAL_CONSECUTIVE_CRITERION_H_ */
