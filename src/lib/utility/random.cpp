#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cstdlib> 		/* RAND_MAX */
#include <limits>       /* min, max of float, double */

#include "random.h"


namespace GeneticLibrary {
namespace Utility {

random::random() {
	//Initialize randomness
	srand (static_cast <unsigned> (time(0)));
}


bool random::generateBit() {
	return rand()&1;
}

int random::generateInt(){
	return rand();
}

int random::generateInt(const int min, const int max){
	 return rand() % (max - min + 1) + min;
}
double random::generateDouble(const double min, const double max){
    double range = (max - min);
    double div = RAND_MAX / range;
    double random = min + (rand() / div);
    return random;
}

double random::generateDouble(){
	return generateDouble(
			std::numeric_limits<double>::min(),
			std::numeric_limits<double>::max());
}

float random::generateFloat(const float min, const float max){
	float range = (max - min);
	float div = ((float) RAND_MAX) / range;
	float random = min + (((float) rand()) / div);
    return random;
}

float random::generateFloat(){
	return generateFloat(
			std::numeric_limits<float>::min(),
			std::numeric_limits<float>::max());
}

} /* namespace Utility */
} /* namespace GeneticLibrary */
