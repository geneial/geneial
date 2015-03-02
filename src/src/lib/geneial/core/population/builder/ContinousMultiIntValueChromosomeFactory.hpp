#ifndef CONT_MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_
#define CONT_MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_

#include <geneial/core/population/builder/ContinousMultiIntValueChromosomeFactory.h>
#include <geneial/utility/Random.h>

#include <algorithm>
#include <cassert>

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {

template <typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::ptr ContinousMultiIntValueChromosomeFactory<FITNESS_TYPE>::createChromosome(bool populateValues){

	using namespace GeneticLibrary::Utility;

	typename MultiValueChromosome<int,FITNESS_TYPE>::ptr new_chromosome(new MultiValueChromosome<int,FITNESS_TYPE>(this->_settings->getFitnessEvaluator()));
	assert(new_chromosome->getSize() == 0);

	new_chromosome->getContainer().reserve(this->_settings->getNum());
	if(populateValues)
	{
		const unsigned int amount = this->_settings->getNum();

		unsigned int i = amount;

		int lastVal; //reference to last inserted value

		while(i--)
		{

			if(i == amount - 1)
			{
				if(_settings->hasStart())
				{
					lastVal = _settings->getStart();
				}
				else
				{
					lastVal = Random::instance()->generateInt(this->_settings->getRandomMin(),this->_settings->getRandomMax());
				}
			}

			const int val = Random::instance()->generateInt(lastVal - _settings->getEps(),lastVal + _settings->getEps());
			const int lower_limited = std::max(this->_settings->getRandomMin(),val);
			const int upper_limited = std::min(this->_settings->getRandomMax(),lower_limited);

			new_chromosome->getContainer().push_back(upper_limited);

			lastVal = upper_limited;

		}
		assert(new_chromosome->getSize() == this->_settings->getNum());

	}

	typename BaseChromosome<FITNESS_TYPE>::ptr result = boost::dynamic_pointer_cast<BaseChromosome<FITNESS_TYPE> >(new_chromosome);

	return result;
}

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* CONT_MULTI_INT_VALUE_CHROMOSOME_FACTORY_HPP_ */
