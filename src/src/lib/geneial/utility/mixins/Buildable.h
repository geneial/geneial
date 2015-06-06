#pragma once

#include <memory>

#include <geneial/namespaces.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

geneial_export_namespace
{

template<typename C>
class Buildable
{
public:
    using ptr = std::shared_ptr<C>;
    using const_ptr = std::shared_ptr<const C>;

    class Builder
    {
    public:
        Builder()
        {
        }

        virtual ~Builder()
        {
        }

        virtual ptr create() = 0;
    };
};


} /* export namespace */
} /* namespace utility */
} /* namespace geneial */
