#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cstdlib> 		/* RAND_MAX */
#include <limits>       /* min, max of float, double */

#include <geneial/utility/random.h>
#include "assert.h"

namespace GeneticLibrary {
namespace Utility {

random::random() {
	//Initialize randomness
	srand (static_cast <unsigned> (time(0)));
}


bool random::generateBit() const {
	return rand()&1;
}


bool random::decision(const double probability) const{
	assert(0.0 <= probability);
	assert(1.0 >= probability);
	const double reference = generateDouble(0.0,1.0);
	if(probability == 0.0)
	{
		return false;
	}else{
		return (reference <= probability);
	}
}

int random::generateInt() const{
	return rand();
}

int random::generateInt(const int min, const int max)const{
	 return rand() % (max - min + 1) + min;
}

double random::generateDouble(const double min, const double max)const{
    double range = (max - min);
    double div = RAND_MAX / range;
    double random = min + (rand() / div);
    return random;
}

double random::generateDouble() const{
	return generateDouble(
			std::numeric_limits<double>::min(),
			std::numeric_limits<double>::max());
}

float random::generateFloat(const float min, const float max) const{
	float range = (max - min);
	float div = ((float) RAND_MAX) / range;
	float random = min + (((float) rand()) / div);
    return random;
}

float random::generateFloat() const{
	return generateFloat(
			std::numeric_limits<float>::min(),
			std::numeric_limits<float>::max());
}

} /* namespace Utility */
} /* namespace GeneticLibrary */
