/*
 * MultiValueNPointCrossover.h
 *
 *  Created on: Dec 12, 2014
 *      Author: bewo
 */

#ifndef MULTIVALUE_CHROMOSOME_N_POINT_CROSSOVER_HPP_
#define MULTIVALUE_CHROMOSOME_N_POINT_CROSSOVER_HPP_

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>

using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Operation::Coupling;

namespace GeneticLibrary {
namespace Operation {
namespace Crossover {

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set MultiValueChromosomeNPointCrossover<VALUE_TYPE,FITNESS_TYPE>::doCrossover(
		typename BaseChromosome<FITNESS_TYPE>::ptr mommy,
		typename BaseChromosome<FITNESS_TYPE>::ptr daddy){

	typedef typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::value_container value_container;
	typedef typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr mvc_ptr;

	typename Coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set resultset;

	mvc_ptr mvc_mommy
			= boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> >(mommy);
	assert(mvc_mommy);

	mvc_ptr mvc_daddy
			= boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> >(daddy);
	assert(mvc_daddy);

	unsigned int crossoverPoints = this->getCrossoverSettings()->getCrossOverPoints();
	unsigned int crossoverWidth = this->getBuilderSettings()->getNum() / crossoverPoints;

	mvc_ptr child_candidate =
			boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> >(
					this->getBuilderFactory()->createChromosome()
			);
	assert(child_candidate);

	value_container &daddy_container = mvc_daddy->getContainer();
	value_container &mommy_container = mvc_mommy->getContainer();
	value_container &child_container = child_candidate->getContainer();

	child_container.clear();

	assert(daddy_container.size() == mommy_container.size());

	typename value_container::iterator mommy_it = mommy_container.begin();
	typename value_container::iterator daddy_it = daddy_container.begin();
	bool flip = true; //ladies first.

	for (	unsigned int i=0;
			mommy_it != mommy_container.end();
			++i){

		if(flip){
			child_container.push_back(*mommy_it);
		}else{
			child_container.push_back(*daddy_it);
		}

		if(i==crossoverWidth){
			i=0;
			flip = !flip;
		}
		++mommy_it;
		++daddy_it;
	}

	resultset.push_back(child_candidate);
	return resultset;
}

} /* namespace Crossover */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#endif /* MULTIVALUE_CHROMOSOME_N_POINT_CROSSOVER_HPP_ */
