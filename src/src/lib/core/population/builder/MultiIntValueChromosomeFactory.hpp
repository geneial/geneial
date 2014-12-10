/*
 * MultiValueChromosomeFactory.h
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#ifndef MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_
#define MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_

#include <core/population/builder/MultiIntValueChromosomeFactory.h>
#include <core/population/chromosome/MultiValueChromosome.h>

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

template <typename FITNESS_TYPE>
BaseChromosome<FITNESS_TYPE> *MultiIntValueChromosomeFactory<FITNESS_TYPE>::createChromosome(){

	using namespace GeneticLibrary::Utility;

	MultiValueChromosome<int,FITNESS_TYPE> *new_chromosome = new MultiValueChromosome<int,FITNESS_TYPE>();
	assert(new_chromosome->getSize() == 0);

	new_chromosome->getContainer().reserve(this->_settings.getNum());

	int i = this->_settings.getNum();
	while(i--){
		new_chromosome->getContainer().push_back(random::instance()->generateInt(this->_settings.getRandomMax(),this->_settings.getRandomMin()));
	}
	assert(new_chromosome->getSize() == this->_settings.getNum());

	//BaseChromosome<FITNESS_TYPE> *result = *new_chromosome;
	return new_chromosome;
}

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_ */
