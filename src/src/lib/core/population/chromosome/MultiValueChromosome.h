/*
 * MultiValueChromosome.h
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#ifndef MULTIVALUECHROMOSOME_H_
#define MULTIVALUECHROMOSOME_H_

#include <population/chromosome/BaseChromosome.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <numeric>

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

using namespace std;

template <typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosome: public BaseChromosome<FITNESS_TYPE> {
public:
	typedef std::vector<VALUE_TYPE> value_container;
	typedef value_container::const_iterator const_it;
	typedef value_container::iterator it;


	MultiValueChromosome() :_container(){};
	virtual ~MultiValueChromosome();

	const_it getConstIt() const;
	it getIt() const;

	unsigned int getSize() const;

	it getMax() const;
	it getMin() const;

	VALUE_TYPE getAverage() const;
	VALUE_TYPE getRange() const;
	VALUE_TYPE getSum() const;

	//TODO (bewo) : provide further convenience methods at this point.

	value_container& getContainer() const;
	void setValueContainer(value_container container);

	virtual void print(std::ostream& os) const;

private:
	value_container _container;

};

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#include "MultiValueChromosome.hpp"
#endif /* MULTIVALUECHROMOSOME_H_ */
