#include "patterns/singleton.hpp"

#ifndef RANDOM_H_
#define RANDOM_H_

namespace GeneticLibrary {
namespace Utility {

class random : public singleton<random> {
	 friend class singleton <random>;
 public:
	bool generateBit();

	int generateInt();
	int generateInt(const int min, const int max);

	double generateDouble();
	double generateDouble(const double min, const double max);

	float generateFloat();
	float generateFloat(const float min, const float max);

	virtual ~random() {};


 protected:
	random();


};

} /* namespace Utility */
} /* namespace GeneticLibrary */

#endif /* RANDOM_H_ */
