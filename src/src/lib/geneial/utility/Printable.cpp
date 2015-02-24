#include <geneial/utility/Printable.h>
#include <cassert>

namespace GeneticLibrary {
namespace Utility {

using namespace GeneticLibrary::Utility;

std::ostream& operator<<(std::ostream& os, const Printable& p)
{
	p.print(os);
	return os;
}

std::ostream& operator<<(std::ostream& os, const Printable*& p)
{
	assert(p);
	p->print(os);
	return os;
}

}
}
