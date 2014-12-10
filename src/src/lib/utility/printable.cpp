#include "printable.h"

#include <cassert>

using namespace GeneticLibrary::Utility;
namespace GeneticLibrary {
namespace Utility {
std::ostream& operator<<(std::ostream& os, const printable& p) {
	p.print(os);
	return os;
}

std::ostream& operator<<(std::ostream& os, const printable*& p) {
	assert(p);
	p->print(os);
	return os;
}
}
}
