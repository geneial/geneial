#include <geneial/utility/Printable.h>
#include <cassert>

using namespace GeneticLibrary::Utility;
namespace GeneticLibrary {
namespace Utility {

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
