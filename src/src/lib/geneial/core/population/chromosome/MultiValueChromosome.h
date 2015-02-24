/*
 * MultiValueChromosome.h
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#ifndef MULTIVALUECHROMOSOME_H_
#define MULTIVALUECHROMOSOME_H_

#include <geneial/core/population/chromosome/BaseChromosome.h>
#include "boost/shared_ptr.hpp"

#include <cassert>
#include <vector>
#include <algorithm>
#include <numeric>

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

template <typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosome: public BaseChromosome<FITNESS_TYPE> {
public:

	typedef typename BaseChromosome<FITNESS_TYPE>::chromsome_hash chromsome_hash;

	typedef typename std::vector<VALUE_TYPE> value_container;
	typedef typename value_container::const_iterator const_it;
	typedef typename value_container::iterator it;

	typedef typename boost::shared_ptr<MultiValueChromosome <VALUE_TYPE,FITNESS_TYPE> > ptr;
	typedef typename boost::shared_ptr<const MultiValueChromosome <VALUE_TYPE,FITNESS_TYPE> > const_ptr;

	MultiValueChromosome(typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator) : BaseChromosome<FITNESS_TYPE>(fitnessEvaluator), _container(){};
	virtual ~MultiValueChromosome() {};

	bool equals(typename BaseChromosome<FITNESS_TYPE>::const_ptr chromosome) const;

	const_it getConstIt() const;
	it getIt() const;

	unsigned int getSize() const;

	it getMax() const;
	it getMin() const;

	VALUE_TYPE getAverage() const;
	VALUE_TYPE getRange() const;
	VALUE_TYPE getSum() const;

	//TODO (bewo) : provide further convenience methods at this point.

	value_container& getContainer();
	value_container getContainer() const;
	void setValueContainer(value_container container);

	virtual void print(std::ostream& os) const;

	virtual chromsome_hash getHash() const;

private:
	value_container _container;

};

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#include <geneial/core/population/chromosome/MultiValueChromosome.hpp>
#endif /* MULTIVALUECHROMOSOME_H_ */
