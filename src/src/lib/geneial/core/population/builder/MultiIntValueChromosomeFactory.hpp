/*
 * MultiValueChromosomeFactory.h
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#ifndef MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_
#define MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_

#include <geneial/core/population/builder/MultiIntValueChromosomeFactory.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <geneial/utility/Random.h>
#include <cassert>

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

template <typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr MultiIntValueChromosomeFactory<FITNESS_TYPE>::createChromosome(){

	using namespace GeneticLibrary::Utility;

	typename MultiValueChromosome<int,FITNESS_TYPE>::ptr new_chromosome(new MultiValueChromosome<int,FITNESS_TYPE>(this->_settings->getFitnessEvaluator()));
	assert(new_chromosome->getSize() == 0);

	new_chromosome->getContainer().reserve(this->_settings->getNum());

	int i = this->_settings->getNum();
	while(i--){
		new_chromosome->getContainer().push_back(Random::instance()->generateInt(this->_settings->getRandomMax(),this->_settings->getRandomMin()));
	}
	assert(new_chromosome->getSize() == this->_settings->getNum());

	typename BaseChromosome<FITNESS_TYPE>::ptr result = boost::dynamic_pointer_cast<BaseChromosome<FITNESS_TYPE> >(new_chromosome);
	return result;
}

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_ */
