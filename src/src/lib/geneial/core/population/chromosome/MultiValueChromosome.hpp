/*
 * MultiValueChromosome.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#ifndef MULTIVALUECHROMOSOME_HPP_
#define MULTIVALUECHROMOSOME_HPP_

#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <iterator>
#include <algorithm>

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

template <typename VALUE_TYPE, typename FITNESS_TYPE>
bool MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::equals(typename BaseChromosome<FITNESS_TYPE>::const_ptr chromosome) const{
	return this->hashEquals(chromosome);
}


template <typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::chromsome_hash MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getHash() const
{

	typename BaseChromosome<FITNESS_TYPE>::chromsome_hash result = 0; //(long) getSum();
	const int hash_bytes = sizeof(chromsome_hash);

	//TODO (bewo): Tests this on doubles, etc.
	const int value_bytes = sizeof(VALUE_TYPE);

	unsigned int shift_bit = 0;
	unsigned int shift_byte = 0;

	//NOTE(bewo): simple hash function
	//1.) Iterate over all values in the chromosomes.
	//2.) XOR the bits of the values with a circular bitshift - bytewise with a circular byteshift to the hash memory
	for (MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::const_it it = _container.begin() ; it != _container.end(); ++it){
		VALUE_TYPE val = *it;
		for(int i=0;i<value_bytes;i++){
			char* ptr_hash = ((char*)&result)+((i+shift_byte)%hash_bytes);
			char* ptr_value = ((char*)&val)+i;
			//NOTE(bewo): xor what was previously there with a circular shift.
			*ptr_hash ^= (*ptr_value << shift_bit) | (*ptr_value >> (sizeof(char) * 8 - shift_bit));
			shift_bit = (shift_bit + 1) % sizeof(char) * CHAR_BIT;
		}
		shift_byte++;
	}


	return result;
}


template <typename VALUE_TYPE, typename FITNESS_TYPE>
unsigned int MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getSize() const{
	return _container.size();
}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::value_container& MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getContainer(){
	return _container;
}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
void MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::setValueContainer(value_container container){
	_container = container;
}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::const_it MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getConstIt() const{
	return _container.begin();
}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::it MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getIt() const{
	return _container.begin();
}

template <typename VALUE_TYPE, typename FITNESS_TYPE>
typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::it MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getMax() const{
	    return max_element(_container.begin(),_container.end());
}
template <typename VALUE_TYPE, typename FITNESS_TYPE>
typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::it MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::getMin() const{
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
	os << "Fitness: ";
	if(this->hasFitness()){
		os << *this->getFitness();
	}else{
		os << "N/A";
	}
	os << ", ";
	os << "Age: " << this->getAge() << ", ";

	os << "Hash: " ;
	this->printHash(os);
	os << std::endl;

	os << "Values: " << std::endl;
	std::ostream_iterator<VALUE_TYPE> out_it(os, "; ");
	std::copy ( _container.begin(), _container.end(), out_it );
	os << std::endl;
}

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */


#endif /* MULTIVALUECHROMOSOME_HPP_ */
