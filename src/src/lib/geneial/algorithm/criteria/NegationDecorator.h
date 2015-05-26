#pragma once

#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>
#include <geneial/core/population/Population.h>


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

} /* geneial_export_namespace */
} /* private namespace stopping_criteria */
} /* private namespace algorithm */
} /* private namespace geneial */
