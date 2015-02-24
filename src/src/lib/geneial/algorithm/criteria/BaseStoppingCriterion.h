/*
 * BaseStopCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef BASESTOPPINGCRITERION_H_
#define BASESTOPPINGCRITERION_H_

#include <geneial/core/population/management/BaseManager.h>

namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

using namespace GeneticLibrary::Population::Manager;

template <typename FITNESS_TYPE>
class BaseStoppingCriterion: public Utility::Printable,
							     public boost::enable_shared_from_this<BaseChromosome <FITNESS_TYPE> >
{
public:
	typedef typename boost::shared_ptr<BaseStoppingCriterion <FITNESS_TYPE> > ptr;

	typedef typename boost::shared_ptr<const BaseStoppingCriterion <FITNESS_TYPE> > const_ptr;

	virtual ~BaseStoppingCriterion(){};


	virtual void print(std::ostream& os) const = 0;
	
	//Should return true, if criterion was met. Can use manager to compute population information.
	//NOTE(bewo): This function must be free of any side-effects and might be called multipe times for the same generation!
	virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager) = 0;
};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* BASESTOPPINGCRITERION_H_ */
