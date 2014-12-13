/*
 * SimpleCoupling.h
 *
 *  Created on: Dec 12, 2014
 *      Author: bewo
 */

#ifndef SIMPLECOUPLING_HPP_
#define SIMPLECOUPLING_HPP_

#include <geneial/core/operations/coupling/SimpleCouplingOperation.h>


using namespace GeneticLibrary::Population::Chromosome;

namespace GeneticLibrary {
namespace Operation {
namespace Coupling {

template<typename FITNESS_TYPE>
typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set SimpleCouplingOperation<FITNESS_TYPE>::doCopulate(
		typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &mating_pool,
		BaseCrossoverOperation<FITNESS_TYPE> *crossoverOperation,
		BaseManager<FITNESS_TYPE> &manager) {

	typedef typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set offspring_container;
	typedef typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set children_container;
	typedef typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set mating_container;
	typedef typename BaseChromosome<FITNESS_TYPE>::ptr chrom_ptr;


	offspring_container offspring; //A small container for all the little children :-)

	unsigned int offspring_left =this->getSettings()->getNumberOfOffspring();

	assert(mating_pool.size()>1);
	//iterate over mating candidates and create a pairwise mapping.
	/**
	 * Example getNumberOfOffspring: 8, #mating_pool = 5
	 *
	 *  Parent1 \/ Child1
	 *  Parent2 /\ Child2
	 *
	 *  Parent3 \/ Child3
	 *  Parent4 /\ Child4
	 *
	 *  Parent5 \/ Child5
	 *  Parent1 /\ Child7
	 *
	 *  Parent2 \/ Child8
	 *  Parent3 /
 	 */
	for (typename mating_container::iterator it = mating_pool.begin(); offspring_left > 0;)
	{
		//TODO (bewo) REFACTOR: research cyclic iterators to avoid all those ugly case distinctions
		chrom_ptr parent1 = *it;
		it++;
		//wrap around if necessary
		if (it == mating_pool.end()) {
			it = mating_pool.begin();
		}
		chrom_ptr parent2 = *it;
		it++;
		if (it == mating_pool.end()) {
			it = mating_pool.begin();
		}

		//TODO (bewo) BEAUTIFY: use copy+backinserter and min(vec.size(),offspring_left) ?!
		children_container children1 = crossoverOperation->doCrossover(parent1,parent2);
		for (typename children_container::iterator it = children1.begin(); offspring_left > 0 && it != children1.end();++it){
			offspring.push_back(*it);
			offspring_left--;
		}
		if(offspring_left){
			 children_container children2 = crossoverOperation->doCrossover(parent2,parent1);
			for (typename children_container::iterator it = children2.begin(); offspring_left > 0 && it != children2.end();++it){
				offspring.push_back(*it);
				offspring_left--;
			}
		}
	}

	return offspring;
}

}
}
}

#endif
