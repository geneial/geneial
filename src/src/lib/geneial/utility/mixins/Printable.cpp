#include <geneial/utility/mixins/Printable.h>
#include <cassert>

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

geneial_export_namespace
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


} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */
