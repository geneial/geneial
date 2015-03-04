#include <geneial/utility/patterns/singleton.hpp>

#ifndef __GENEIAL_RANDOM_H_
#define __GENEIAL_RANDOM_H_

namespace geneial {
namespace utility {

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

} /* namespace utility */
} /* namespace geneial */

#endif /* __GENEIAL_RANDOM_H_ */
