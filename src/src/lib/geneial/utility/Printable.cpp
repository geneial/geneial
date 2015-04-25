#include <geneial/utility/Printable.h>
#include <cassert>

namespace geneial
{
namespace utility
{

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

} /* namespace utility */
} /* namespace geneial */
