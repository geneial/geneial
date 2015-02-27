/*
 * common_casts.h
 *
 *  Created on: Feb 27, 2015
 *      Author: bewo
 */

#ifndef COMMON_CASTS_H_
#define COMMON_CASTS_H_

#include <boost/shared_ptr.hpp>
#include <boost/test/unit_test.hpp>

#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>

namespace TestHelper{

using namespace GeneticLibrary::Population::Chromosome;

template <typename VALUE_TYPE, typename FITNESS_TYPE>
inline typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr convertBaseChromosome(typename BaseChromosome<FITNESS_TYPE>::ptr ptr)
{
	typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr myChrom
		= boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> >(ptr);
	BOOST_CHECK(myChrom);
	return myChrom;
}

}
#endif /* COMMON_CASTS_H_ */
