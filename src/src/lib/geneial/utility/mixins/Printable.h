#pragma once

#include <geneial/namespaces.h>

#include <iostream>


geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

geneial_export_namespace
{

class Printable
{
public:
    virtual ~Printable()
    {
    }

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

} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */
