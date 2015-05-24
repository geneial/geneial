#pragma once

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/StatefulStoppingCriterion.h>
#include <deque>

namespace __geneial_noexport
{
namespace __algorithm_impl
{
namespace __stopping_criteria_impl
{
using ::geneial::population::management::BaseManager;

inline namespace exports
{
using namespace ::geneial::algorithm::stopping_criteria;
/**
 * Decorator that checks whether a given criterion has been reached several times.
 */
template<typename FITNESS_TYPE>
class ConsecutiveDecorator: public StatefulStoppingCriterion<FITNESS_TYPE>
{

public:
    using criterion = BaseStoppingCriterion<FITNESS_TYPE>;

    ConsecutiveDecorator(const unsigned int windowSize, const unsigned int consecutiveHits,
            const std::shared_ptr<criterion> & criterion) :
            _criterion(criterion), _windowSize(windowSize), _consecutiveHits(consecutiveHits)
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
        os << "Consecutive (criterion:" << *_criterion << ", win: " << _windowSize << ")";
    }

    const criterion& getCriterion() const
    {
        return *_criterion;
    }

    void setCriterion(const std::shared_ptr<criterion> & criterion)
    {
        _criterion = criterion;
    }

private:
    std::shared_ptr<criterion> _criterion;
    const unsigned int _windowSize;
    const unsigned int _consecutiveHits;
    std::deque<bool> _window;

};

} /* namespace exports */
} /* namespace __stopping_criteria_impl */
} /* namespace __algorithm_impl */
} /* namespace __geneial_noexport */

