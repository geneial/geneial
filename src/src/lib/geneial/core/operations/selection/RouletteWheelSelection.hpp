#pragma once

#include <geneial/core/operations/selection/RouletteWheelSelection.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/utility/Random.h>

#include <vector>
#include <algorithm>
#include <cassert>
#include <utility>

namespace geneial
{
namespace operation
{
namespace selection
{

//TODO (bewo): This is not very efficient for large populations.

template<typename FITNESS_TYPE>
class RouletteWheel
{

private:

    typedef typename BaseChromosome<FITNESS_TYPE>::ptr chrom_ptr_type;

    typedef typename Population<FITNESS_TYPE>::fitnessmap_const_it const_pop_itr;

    FITNESS_TYPE _sum;

    //NOTE (bewo): Benchmarked this against a map, however the RBTree and all the mallocs are very costly, hence vector.
    std::vector<std::pair<FITNESS_TYPE, chrom_ptr_type>> _ranges;

    struct RouletteWheelComparator
    {
        bool operator()(const std::pair<FITNESS_TYPE, chrom_ptr_type>& v1,
                const std::pair<FITNESS_TYPE, chrom_ptr_type>& v2) const
        {
            return v1.first < v2.first;
        }

        bool operator()(const std::pair<FITNESS_TYPE, chrom_ptr_type>& v1, const FITNESS_TYPE v2) const
        {
            return v1.first < v2;
        }

        bool operator()(const FITNESS_TYPE v1, const FITNESS_TYPE v2) const
        {
            return v1 < v2;
        }

        bool operator()(const FITNESS_TYPE v1, const std::pair<FITNESS_TYPE, chrom_ptr_type>& v2) const
        {
            return v1 < v2.first;
        }
    };

public:
    //minimal place on the wheel
    constexpr const static FITNESS_TYPE CONST_INC_BY = 1;

    RouletteWheel(const Population<FITNESS_TYPE> &population) :
            _sum(0)
    {

        _ranges.reserve(population.getSize());

        //Worst value:
        const FITNESS_TYPE worstFitness = population.getFitnessMap().cbegin()->first;
        FITNESS_TYPE positiveTranslation = 0;

        if(worstFitness <= 0)
        {
            positiveTranslation = std::abs(worstFitness);
        }

        for (const auto &it : population.getFitnessMap())
        {
            _sum += CONST_INC_BY;

            if(it.first < 0)
            {
                _sum += std::abs(it.first);
            }
            else
            {
                //In case we had negative fitness values in the population,
                //we offset of the positive fitness values by the amount of the worst negative fitness,
                //so that the positive fitness values have higher probability than the negative ones.
                _sum += positiveTranslation + it.first;
            }

            _ranges.push_back(std::pair<FITNESS_TYPE, chrom_ptr_type>(_sum, it.second));
        }
    }

    chrom_ptr_type spin(FITNESS_TYPE random)
    {
        return std::lower_bound(_ranges.begin(), _ranges.end(), random * _sum, RouletteWheelComparator())->second;
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
            const FITNESS_TYPE random = (FITNESS_TYPE) Random::generate<FITNESS_TYPE>(0.0, 1.0);
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
