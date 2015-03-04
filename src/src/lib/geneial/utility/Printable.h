#ifndef __GENEIAL_PRINTABLE_H_
#define __GENEIAL_PRINTABLE_H_
#include <iostream>

namespace geneial {
namespace utility {

class Printable {
public:
	virtual ~Printable() {};

	virtual void print(std::ostream& os) const = 0;
};
/**
 * Output as a stream of characters. Calls the print() method of the class.
 */
std::ostream& operator<<(std::ostream& os, const Printable& p);
/**
 * Output as a stream of characters. Calls the print() method of the class.
 */
std::ostream& operator<<(std::ostream& os, const Printable*& p);


} /* namespace utility */
} /* namespace geneial */
#endif /* __GENEIAL_PRINTABLE_H_ */
