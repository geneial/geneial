#pragma once

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>

#include <list>
#include <map>

namespace geneial
{
namespace algorithm
{
namespace stopping_criteria
{

using namespace geneial::population::management;

//Container that holds other Criteria which are connected by a logical condition (and/or), which propagate the condition by visitor pattern or sth.
//Composite Pattern for hierarchies of criteria
//Associativity: left
template<typename FITNESS_TYPE>
class CombinedCriterion: public BaseStoppingCriterion<FITNESS_TYPE>
{
public:
    enum glue
    {
        INIT, AND, OR, XOR
    };

    typedef typename BaseStoppingCriterion<FITNESS_TYPE>::ptr criterion;

    typedef std::pair<glue, criterion> glue_criterion_pair;

    typedef std::list<glue_criterion_pair> container;

    virtual ~CombinedCriterion()
    {
    }

    /**
     * Returns true if empty.
     */
    virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager)
    {
        bool result = true;

        assert(std::count_if(_criteria.begin(), _criteria.end(), [](glue_criterion_pair const &b)
        {
            return b.first == INIT;
        }) != 1

        && "INIT type not found or there are more than one INIT glue for combined criterion!");

        for (typename container::iterator it = _criteria.begin(); it != _criteria.end(); ++it)
        {
            if (it->first == INIT)
            {
                result = it->second->wasReached(manager);
            }
            else if (it->first == AND)
            {
                result &= it->second->wasReached(manager);
            }
            else if (it->first == XOR)
            {
                result &= !it->second->wasReached(manager);
            }
            else
            {
                result |= it->second->wasReached(manager);
            }
        }
        return result;
    }

    virtual void print(std::ostream& os) const
    {
        os << "Combined (";
        for (typename container::const_iterator it = _criteria.begin(); it != _criteria.end(); ++it)
        {
            if (it->first == AND)
            {
                os << "(&&) ";
            }
            else if (it->first == XOR)
            {
                os << "(^^) ";
            }
            else
            {
                os << "(||) ";
            }
            os << *it->second;
        }
        os << ")";
    }

    void add(const glue_criterion_pair newCriterion)
    {
        _criteria.insert(_criteria.end(), newCriterion);
    }

    void add(const glue glue, const criterion criterion)
    {
        add(glue_criterion_pair(glue, criterion));
    }

    const container& getCriteria() const
    {
        return _criteria;
    }

    void setCriteria(const container& criteria)
    {
        _criteria = criteria;
    }

private:
    container _criteria;

};

} /* namespace stopping_criteria */
} /* namespace algorithm */
} /* namespace geneial */

