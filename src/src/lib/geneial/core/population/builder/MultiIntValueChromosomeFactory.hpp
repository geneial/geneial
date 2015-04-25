#ifndef __GENEIAL_MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_
#define __GENEIAL_MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_

#include <geneial/core/population/builder/MultiIntValueChromosomeFactory.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <geneial/utility/Random.h>

#include <cassert>

namespace geneial
{
namespace population
{
namespace chromosome
{

template<typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr MultiIntValueChromosomeFactory<FITNESS_TYPE>::createChromosome(
        typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues)
{

    typename MultiValueChromosome<int, FITNESS_TYPE>::ptr new_chromosome(
            new MultiValueChromosome<int, FITNESS_TYPE>(this->_settings->getFitnessEvaluator()));
    assert(new_chromosome->getSize() == 0);

    new_chromosome->getContainer().reserve(this->_settings->getNum());

    if (populateValues == BaseChromosomeFactory<FITNESS_TYPE>::CREATE_VALUES)
    {
        int i = this->_settings->getNum();
        while (i--)
        {
            new_chromosome->getContainer().push_back(
                    Random::instance()->generateInt(this->_settings->getRandomMin(), this->_settings->getRandomMax()));
        }
        assert(new_chromosome->getSize() == this->_settings->getNum());

    }

    typename BaseChromosome<FITNESS_TYPE>::ptr result = boost::dynamic_pointer_cast<BaseChromosome<FITNESS_TYPE> >(
            new_chromosome);

    return result;
}

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

#endif /* __GENEIAL_MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_ */
