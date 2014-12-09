/*
 * Fitness.hpp
 *
 *  Created on: Dec 8, 2014
 *      Author: bewo
 */
#ifndef FITNESS_HPP_
#define FITNESS_HPP_
#include "Fitness.h"

template <typename FITNESS_TYPE> FITNESS_TYPE GeneticLibrary::Fitness<FITNESS_TYPE>::get() const {
  return _value;
}

template <typename FITNESS_TYPE> void GeneticLibrary::Fitness<FITNESS_TYPE>::set(FITNESS_TYPE value) {
  _value = value;
}

template <typename FITNESS_TYPE> void GeneticLibrary::Fitness<FITNESS_TYPE>::print(std::ostream& os) const{
	os << _value;
}

#endif /* FITNESS_HPP_ */
