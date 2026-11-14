#pragma once

#include <geneial/algorithm/criteria/StatefulStoppingCriterion.h>

#include <deque>
#include <stdexcept>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(algorithm)
    {
        geneial_private_namespace(stopping_criteria)
        {
            using ::geneial::population::management::BaseManager;

            geneial_export_namespace
            {
                /**
                 * Counts hits within a sliding observation window, not necessarily adjacent hits.
                 */
                template <typename FITNESS_TYPE>
                class ConsecutiveDecorator : public StatefulStoppingCriterion<FITNESS_TYPE>
                {

                  public:
                    using criterion = BaseStoppingCriterion<FITNESS_TYPE>;

                    ConsecutiveDecorator(const unsigned int windowSize, const unsigned int consecutiveHits,
                                         const std::shared_ptr<criterion> &criterion)
                        : _criterion(criterion), _windowSize(windowSize), _consecutiveHits(consecutiveHits)
                    {
                        if (!criterion || windowSize == 0 || consecutiveHits == 0 || consecutiveHits > windowSize)
                            throw std::invalid_argument("A criterion and a valid hit/window count are required");
                    }

                    virtual ~ConsecutiveDecorator() {}

                    void reset() override
                    {
                        StatefulStoppingCriterion<FITNESS_TYPE>::reset();
                        _window.clear();
                    }

                    bool wasStatefullyReached(BaseManager<FITNESS_TYPE> &manager) override
                    {
                        updateWindowValues(manager);

                        bool result = false;

                        std::deque<bool>::iterator it = _window.begin();
                        // run over deque, count how often the criteria was met.
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

                  public:
                    void print(std::ostream &os) const override
                    {
                        os << "Consecutive (criterion:" << *_criterion << ", win: " << _windowSize << ")";
                    }

                    const criterion &getCriterion() const
                    {
                        return *_criterion;
                    }

                    void setCriterion(const std::shared_ptr<criterion> &criterion)
                    {
                        if (!criterion)
                            throw std::invalid_argument("Criterion cannot be null");
                        _criterion = criterion;
                        reset();
                    }

                  private:
                    std::shared_ptr<criterion> _criterion;
                    const unsigned int _windowSize;
                    const unsigned int _consecutiveHits;
                    std::deque<bool> _window;
                };

            } /* geneial_export_namespace */
        } /* private namespace stopping_criteria */
    } /* private namespace algorithm */
} /* private namespace geneial */
