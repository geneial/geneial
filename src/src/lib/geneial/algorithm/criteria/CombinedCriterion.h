/*
 * FitnessDeltaCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef COMBINED_CRITERION_H_
#define COMBINED_CRITERION_H_

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>
#include <list>
#include <map>

namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

using namespace GeneticLibrary::Population::Manager;

//Container that holds other Criteria which are connected by a logical condition (and/or), which propagate the condition by visitor pattern or sth.
//Composite Pattern for hierarchies of criteria
//Associativity: left
template <typename FITNESS_TYPE>
class CombinedCriterion : public BaseStoppingCriterion<FITNESS_TYPE>
{
public:
	enum glue { AND , OR, XOR };

	typedef typename BaseStoppingCriterion<FITNESS_TYPE>::ptr criterion;

	typedef std::pair<glue,criterion> glue_criterion_pair;

	typedef std::list<glue_criterion_pair> container;

	virtual ~CombinedCriterion() {};

	/**
	 * Returns true if empty.
	 */
	virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager)
	{
		bool result = true;
		for(typename container::iterator it = _criteria.begin();it != _criteria.end();++it){
			if(it->first == AND)
			{
				result &= it->second->wasReached(manager);
			}
			else if(it->first == XOR)
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
		for(typename container::const_iterator it = _criteria.begin();it != _criteria.end();++it)
		{
			if(it->first == AND)
			{
				os << "(&&) ";
			}
			else if(it->first == XOR)
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
		_criteria.insert(_criteria.end(),newCriterion);
	}

	void add(const glue glue, const criterion criterion )
	{
		glue_criterion_pair p(glue,criterion);
		add(p);
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

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* COMBINED_CRITERION_H_ */
