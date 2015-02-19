#include <geneial/utility/patterns/singleton.hpp>

#ifndef RANDOM_H_
#define RANDOM_H_

namespace GeneticLibrary {
namespace Utility {

class Random : public singleton<Random> {
	 friend class singleton <Random>;
 public:
	bool generateBit()  const;

	bool decision(const double probability) const;

	int generateInt() const;
	int generateInt(const int min, const int max) const;

	double generateDouble() const;
	double generateDouble(const double min, const double max) const;

	float generateFloat() const;
	float generateFloat(const float min, const float max) const;

	virtual ~Random() {};


 protected:
	Random();


};

} /* namespace Utility */
} /* namespace GeneticLibrary */

#endif /* RANDOM_H_ */
