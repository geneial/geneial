#ifndef PRINTABLE_H_
#define PRINTABLE_H_
#include <iostream>
namespace GeneticLibrary {
namespace Utility {
class printable {
public:
	virtual ~printable() {};
	virtual void print(std::ostream& os) const = 0;
};
/**
 * Output as a stream of characters. Calls the print() method of the class.
 */
std::ostream& operator<<(std::ostream& os, const printable& p);
/**
 * Output as a stream of characters. Calls the print() method of the class.
 */
std::ostream& operator<<(std::ostream& os, const printable*& p);

} /* namespace Utility */
} /* namespace GeneticLibrary */
#endif /* PRINTABLE_H_ */
