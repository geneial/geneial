/*
 * FitnessProportionalSelection.h
 *
 *  Created on: Dec 11, 2014
 *      Author: bewo
 */

#ifndef ROULETTEWHEELSELECTION_HPP_
#define ROULETTEWHEELSELECTION_HPP_

#include <geneial/core/operations/selection/RouletteWheelSelection.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/utility/Random.h>

#include <map>
#include <cassert>


namespace GeneticLibrary {
namespace Operation {
namespace Selection {

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Manager;
using namespace GeneticLibrary::Population::Chromosome;
using namespace GeneticLibrary::Utility;

//TODO (bewo) check whether all this will work with negative fitness values

template <typename FITNESS_TYPE>
class RouletteWheelComparator {
public:
    bool operator()(const std::pair<FITNESS_TYPE, FITNESS_TYPE> &a, const std::pair<FITNESS_TYPE, FITNESS_TYPE> &b) {
        if (b.first == -1) {
            if (b.second >= a.first && b.second < a.second)
                return false;
            return (b.second >= a.second);
        }
        if (a.first == -1) {
            if (a.second >= b.first && a.second < b.second)
                return false;
            return (a.second >= b.second);
        }
        return a.first < b.first;
    }
};


template <typename FITNESS_TYPE>
class RouletteWheel {
private:
	typedef typename BaseChromosome<FITNESS_TYPE>::ptr chrom_ptr_type;
	typedef typename Population<FITNESS_TYPE>::const_it const_pop_itr;
	FITNESS_TYPE _sum;
    std::map<std::pair<FITNESS_TYPE, FITNESS_TYPE> , chrom_ptr_type, RouletteWheelComparator<FITNESS_TYPE> > ranges;
public:
    RouletteWheel(const Population<FITNESS_TYPE> &population) :_sum(0){
		for (const_pop_itr it =	population.getChromosomes().begin();
				it != population.getChromosomes().end(); ++it) {
			ranges[std::pair<FITNESS_TYPE, FITNESS_TYPE>(_sum, it->first + _sum)] = it->second;
			_sum += it->first;
		}
    }

    chrom_ptr_type spin(FITNESS_TYPE random) {
        return ranges.find(std::pair<FITNESS_TYPE, FITNESS_TYPE>(-1, random*_sum))->second;
    }
};


template <typename FITNESS_TYPE>
typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set RouletteWheelSelection<FITNESS_TYPE>::doSelect(
		const Population<FITNESS_TYPE> &population,
		BaseManager<FITNESS_TYPE> &manager){

		//shorthands for type mess
		typedef typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set result_set;
		typedef typename BaseChromosome<FITNESS_TYPE>::ptr chrom_ptr_type;

		result_set result;

		unsigned int left_select = this->getSettings()->getNumberOfParents();

		RouletteWheel<FITNESS_TYPE> rouletteWheel(population);

		//TODO (bewo) allow parameter for the best chromosomes to be selected (and skipped here)
		assert(population.getSize()>=left_select);

		while(left_select > 0){
			//TODO (bewo) make this a RouletteWheel setting:
			const bool allowDuplicates = false;
			chrom_ptr_type ptr;
			do{
				//TODO (bewo) this is suboptimal:
				FITNESS_TYPE random = (FITNESS_TYPE) Random::instance()->generateDouble(0.0,1.0);
				ptr = rouletteWheel.spin(random);
			}while(allowDuplicates || std::find(result.begin(), result.end(), ptr)!=result.end());
			left_select--;
			result.push_back(ptr);
		}
		return result;
}


} /* namespace Selection */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#endif /* ROULETTEWHEELSELECTION_HPP_ */
