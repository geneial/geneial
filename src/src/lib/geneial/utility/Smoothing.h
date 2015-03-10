#ifndef __GENEIAL_SMOOTHING_H_
#define __GENEIAL_SMOOTHING_H_

#include <geneial/core/population/chromosome/MultiValueChromosome.h>

namespace geneial {
namespace utility {

//Library for some common smoothing operations

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class Smoothing{

private:
	Smoothing(){};
	virtual ~Smoothing(){};

public:

	//TODO (bewo): Docs!
	static void restoreSmoothness(
			typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr chromosome,
			VALUE_TYPE maxAbsElevation,
			VALUE_TYPE min,
			VALUE_TYPE max
			);

	//TODO (bewo): Docs!
	static void peakAt(
			unsigned int pos,
			unsigned int epsLeft,
			unsigned int epsRight,
			VALUE_TYPE elevation,
			typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr chromosome
			);


};

} /* namespace utility */
} /* namespace geneial */

#include <geneial/utility/Smoothing.hpp>

#endif /* __GENEIAL_SMOOTHING_H_ */
