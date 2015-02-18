/*
 * BaseStopCriterion.h
 *
 *  Created on: Dec 10, 2014
 *      Author: bewo
 */

#ifndef BASESTOPPINGCRITERION_H_
#define BASESTOPPINGCRITERION_H_

#include <geneial/core/population/management/BaseManager.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Algorithm {
namespace StoppingCriteria {

template <typename FITNESS_TYPE>
class BaseStoppingCriterion: public Utility::printable,
							 public boost::enable_shared_from_this<BaseChromosome <FITNESS_TYPE> >
{
public:

	typedef typename boost::shared_ptr<BaseStoppingCriterion <FITNESS_TYPE> > ptr;
	typedef typename boost::shared_ptr<const BaseStoppingCriterion <FITNESS_TYPE> > const_ptr;

	virtual ~BaseStoppingCriterion(){};

	virtual void print(std::ostream& os) const = 0;
	
	virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager) = 0;
};

} /* namespace StoppingCriteria */
} /* namespace Algorithm */
} /* namespace GeneticLibrary */

#endif /* BASESTOPPINGCRITERION_H_ */
