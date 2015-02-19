/*
 * MultiValueNPointCrossover.h
 *
 *  Created on: Dec 12, 2014
 *      Author: bewo
 */

#ifndef MULTIVALUE_CHROMOSOME_N_POINT_CROSSOVER_HPP_
#define MULTIVALUE_CHROMOSOME_N_POINT_CROSSOVER_HPP_

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/utility/Random.h>

#include <set>
#include <algorithm>
#include <iterator>

using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Utility;
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

	const unsigned int crossoverPoints = this->getCrossoverSettings()->getCrossOverPoints();
	const unsigned int totalWidth = this->getBuilderSettings()->getNum();
	std::set<unsigned int> crossoverPositions;
	if(this->getCrossoverSettings()->getWidthSetting() == MultiValueChromosomeNPointCrossoverSettings::EQUIDISTANT_WIDTH){

		const unsigned int equidistantwidth = totalWidth / crossoverPoints;

		for(unsigned int i = 0;i<crossoverPoints-1;i++){
			crossoverPositions.insert(i*equidistantwidth);
		}

	}else if(this->getCrossoverSettings()->getWidthSetting() == MultiValueChromosomeNPointCrossoverSettings::RANDOM_WIDTH
			|| this->getCrossoverSettings()->getWidthSetting() == MultiValueChromosomeNPointCrossoverSettings::RANDOM_MIN_WIDTH	){

		unsigned int minWidth;
		if(this->getCrossoverSettings()->getWidthSetting() == MultiValueChromosomeNPointCrossoverSettings::RANDOM_WIDTH){
			minWidth = 1;
		}else{
			minWidth = this->getCrossoverSettings()->getMinWidth();
		}


		for(unsigned int i = 0;i<crossoverPoints;i++){
			//Pick a random number in the interval
			unsigned int rnd_pos;
			bool valid;
			do{
				rnd_pos = Random::instance()->generateInt(0,this->getBuilderSettings()->getNum());

				std::set<unsigned int>::iterator itlow=std::lower_bound(crossoverPositions.begin(),crossoverPositions.end(),rnd_pos);
				if(itlow != crossoverPositions.end()){
					std::advance(itlow,-1);
				}
				std::set<unsigned int>::const_iterator itup=crossoverPositions.upper_bound (rnd_pos);
				std::set<unsigned int>::const_iterator pos=crossoverPositions.find(rnd_pos);

				valid = true;

				valid &= rnd_pos >= minWidth; //ensure there is enough space on the interval boundaries
				if(!valid) {continue;}

				valid &= rnd_pos <= totalWidth - minWidth;
				if(!valid) {continue;}

				valid &= pos == crossoverPositions.end(); //element is not contained within the set
				if(!valid) {continue;}

				//is the picked element too near to another element already contained?
				//ensure between two values there is enough width, i.e. either we have a lower or upper neighbor and the distance is correct or not.
				valid &= itlow == crossoverPositions.end() || (itlow != crossoverPositions.end() && rnd_pos - *itlow >= minWidth);
				if(!valid) {continue;}
				valid &= itup == crossoverPositions.end() || (itup != crossoverPositions.end() && *itup - rnd_pos >= minWidth);
				if(!valid) {continue;}

			}while(!valid);
			crossoverPositions.insert(rnd_pos);
		}
	}
 	assert(crossoverPositions.size()==crossoverPoints);


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
	std::set<unsigned int>::const_iterator widthIterator = crossoverPositions.begin();
		bool flip = true; //copy from ladies first.

		for (unsigned int i=0;
			mommy_it != mommy_container.end();
			++i){

		if(flip){
			child_container.push_back(*mommy_it);
		}else{
			child_container.push_back(*daddy_it);
		}

		if(i==*widthIterator){
			++widthIterator;
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
