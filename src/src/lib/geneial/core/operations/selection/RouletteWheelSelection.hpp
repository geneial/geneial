#pragma once

#include <geneial/core/operations/selection/RouletteWheelSelection.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/utility/Random.h>

#include <map>
#include <unordered_map>
#include <cassert>

namespace geneial
{
namespace operation
{
namespace selection
{

//TODO (bewo): This is not very efficient for large populations.
//TODO (bewo): This seems not to work with negative Fitness values!

template<typename FITNESS_TYPE>
class RouletteWheel
{

private:

    typedef typename BaseChromosome<FITNESS_TYPE>::ptr chrom_ptr_type;

    typedef typename Population<FITNESS_TYPE>::fitnessmap_const_it const_pop_itr;

    FITNESS_TYPE _sum;

    std::map<FITNESS_TYPE, chrom_ptr_type> _ranges;

public:
    //minimal place on the wheel
    constexpr const static FITNESS_TYPE CONST_INC_BY = 1;

    RouletteWheel(const Population<FITNESS_TYPE> &population) :
            _sum(0)
    {
        auto hint = _ranges.end();
        for (const auto &it : population.getFitnessMap())
        {
            _sum += CONST_INC_BY + it.first;
            hint = _ranges.emplace_hint(hint,_sum,it.second);
        }
    }

    chrom_ptr_type spin(FITNESS_TYPE random)
    {
        return _ranges.lower_bound(random * _sum)->second;
    }
};

template<typename FITNESS_TYPE>
typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set RouletteWheelSelection<FITNESS_TYPE>::doSelect(
        const Population<FITNESS_TYPE> &population, BaseManager<FITNESS_TYPE> &manager)
{

    //shorthands for type mess
    typedef typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set result_set;
    typedef typename BaseChromosome<FITNESS_TYPE>::ptr chrom_ptr_type;

    result_set result;

    unsigned int left_select = this->getSettings().getNumberOfParents();

    RouletteWheel<FITNESS_TYPE> rouletteWheel(population);

    //TODO (bewo) allow parameter for the best chromosomes to be selected (and skipped here)
    assert(population.getSize() >= left_select);

    while (left_select > 0)
    {
        //TODO (bewo) make this a RouletteWheel setting:
        const bool allowDuplicates = false;
        chrom_ptr_type ptr;
        do
        {
            //TODO (bewo) this is suboptimal for ints..
            FITNESS_TYPE random = (FITNESS_TYPE) Random::generate<FITNESS_TYPE>(0.0, 1.0);
            ptr = rouletteWheel.spin(random);
        } while (allowDuplicates || std::find(result.begin(), result.end(), ptr) != result.end());
        left_select--;
        result.push_back(ptr);
    }
    return result;
}

} /* namespace selection */
} /* namespace operation */
} /* namespace geneial */
