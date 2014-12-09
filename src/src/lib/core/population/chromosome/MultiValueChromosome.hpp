/*
 * MultiValueChromosome.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#ifndef MULTIVALUECHROMOSOME_HPP_
#define MULTIVALUECHROMOSOME_HPP_

#include <population/chromosome/MultiValueChromosome.h>

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

template <typename VALUE_TYPE, typename FITNESS_TYPE>
MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::MultiValueChromosome() {
}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::~MultiValueChromosome() {
}


template <typename VALUE_TYPE, typename FITNESS_TYPE>
unsigned int MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getSize() const{
	return _container.size();
}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::value_container& MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getContainer() const{
	return _container;
}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
void MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::setValueContainer(value_container container){
	_container = container;
}


template <typename VALUE_TYPE, typename FITNESS_TYPE>
MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::const_it MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getConstIt() const{
	return _container.const_iterator;
}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::it MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getIt() const{
	return _container.iterator;
}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::it MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getMax() const{
	    return max_element(_container.begin(),_container.end());
}
template <typename VALUE_TYPE, typename FITNESS_TYPE>
MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::it MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getMin() const{
	   return min_element(_container.begin(),_container.end());
}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
VALUE_TYPE MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getAverage() const{
	const VALUE_TYPE sum = std::accumulate(_container.begin(),_container.end(),0);
	const unsigned int size = getSize();

	if(size == 0){
		return 0;
	}else{
		assert(size>0);
		return sum/size;
	}
}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
VALUE_TYPE MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getRange() const{
	MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::const_it max = getMax();
	MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::const_it min = getMin();
	assert(max>=min);
	return *max - *min;
}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
VALUE_TYPE MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getSum() const{
	VALUE_TYPE sum = std::accumulate(_container.begin(),_container.end(),0);
	return sum;

}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
void MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::print(std::ostream& os) const{
	os << "Values: " << std::endl;
	std::ostream_iterator<VALUE_TYPE,FITNESS_TYPE> out_it(os, "; ");
	std::copy ( _container.begin(), _container.end(), out_it );
}

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */


#endif /* MULTIVALUECHROMOSOME_HPP_ */
