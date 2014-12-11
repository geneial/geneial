#include "patterns/singleton.hpp"

#ifndef RANDOM_H_
#define RANDOM_H_

namespace GeneticLibrary {
namespace Utility {

class random : public singleton<random> {
	 friend class singleton <random>;
 public:
	bool generateBit()  const;

	bool decision(const double probability) const;

	int generateInt() const;
	int generateInt(const int min, const int max) const;

	double generateDouble() const;
	double generateDouble(const double min, const double max) const;

	float generateFloat() const;
	float generateFloat(const float min, const float max) const;

	virtual ~random() {};


 protected:
	random();


};

} /* namespace Utility */
} /* namespace GeneticLibrary */

#endif /* RANDOM_H_ */
