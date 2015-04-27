#pragma once

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>
#include <geneial/core/population/Population.h>

namespace geneial
{
namespace algorithm
{
namespace stopping_criteria
{

using namespace geneial::population::management;

/**
 * Decorator that negates a criterion
 */
template<typename FITNESS_TYPE>
class NegationDecorator: public BaseStoppingCriterion<FITNESS_TYPE>
{
public:
    typedef typename BaseStoppingCriterion<FITNESS_TYPE>::ptr criterion;

    NegationDecorator(typename BaseStoppingCriterion<FITNESS_TYPE>::ptr criterion) :
            _criterion(criterion)
    {
    }

    virtual ~NegationDecorator()
    {
    }

    virtual inline bool wasReached(BaseManager<FITNESS_TYPE> &manager)
    {
        return !(_criterion->wasReached(manager));
    }

    virtual void print(std::ostream& os) const
    {
        os << "Negated (" << _criterion << ")";
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

};

} /* namespace stopping_criteria */
} /* namespace algorithm */
} /* namespace geneial */

