/*
 * MultiValueChromosomeFactory.h
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#ifndef MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_
#define MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_

#include <core/population/builder/MultiIntValueChromosomeFactory.h>

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {
template <typename FITNESS_TYPE> BaseChromosome<FITNESS_TYPE> *MultiIntValueChromosomeFactory<FITNESS_TYPE>::createChromosome(){

	using namespace GeneticLibrary::Utility;

	MultiValueBuilderSettings<VALUE_TYPE> *settings = &MultiValueChromosomeFactory<int,FITNESS_TYPE>::_settings;
	MultiValueChromosome<int,FITNESS_TYPE> *new_chromosome = new MultiValueChromosome<int,FITNESS_TYPE>();
	assert(new_chromosome->getSize() == 0);

	new_chromosome->getContainer().reserve(settings->getNum());

	int i = settings->getNum();
	while(i--){
		new_chromosome->getContainer().push_back(random::generateInt(settings->getRandomMax(),settings->getRandomMin()));
	}
	assert(new_chromosome->getSize() == MultiValueChromosomeFactory<int,FITNESS_TYPE>::_settings.getNum());

	BaseChromosome<FITNESS_TYPE> *result = *new_chromosome;
	return new_chromosome;
}

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_ */
