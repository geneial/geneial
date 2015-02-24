/*
 * SimpleCoupling.h
 *
 *  Created on: Dec 12, 2014
 *      Author: bewo
 */

#ifndef RANDOMCOUPLING_HPP_
#define RANDOMCOUPLING_HPP_

#include <geneial/core/operations/coupling/RandomCouplingOperation.h>
#include <geneial/utility/Random.h>

#include <algorithm>
#include <iterator>


namespace GeneticLibrary {
namespace Operation {
namespace Coupling {

using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Utility;

template<typename FITNESS_TYPE>
typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set RandomCouplingOperation<FITNESS_TYPE>::doCopulate(
		typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &mating_pool,
		BaseCrossoverOperation<FITNESS_TYPE> *crossoverOperation,
		BaseManager<FITNESS_TYPE> &manager) {

	typedef typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set offspring_container;
	typedef typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set children_container;
	typedef typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set mating_container;


	offspring_container offspring; //A small container for all that little children,we're about to create :-)

	unsigned int offspring_left =this->getSettings()->getNumberOfOffspring();

	assert(mating_pool.size()>1);

	while(offspring_left){

		//mating pool size:
		const unsigned int mating_pool_size = std::distance(mating_pool.begin(), mating_pool.end()) -1;

		//pick a random mommy:
		const unsigned int rnd_mommy = Random::instance()->generateInt(0,mating_pool_size);
		typename mating_container::iterator it_mommy = mating_pool.begin();
		std::advance(it_mommy,rnd_mommy);
		//pick a random daddy:
		unsigned int rnd_daddy;
		do{
			rnd_daddy = Random::instance()->generateInt(0,mating_pool_size);
		}while(rnd_daddy == rnd_mommy);

		typename mating_container::iterator it_daddy = mating_pool.begin();
		std::advance(it_daddy,rnd_daddy);

		//compute crossover
		children_container children1 = crossoverOperation->doCrossover(*it_mommy,*it_daddy);

		for (typename children_container::iterator it = children1.begin(); offspring_left > 0 && it != children1.end();++it){
			offspring.push_back(*it);
			offspring_left--;
		}

	}

	return offspring;
}

}
}
}

#endif /* RANDOMCOUPLING_HPP_ */
