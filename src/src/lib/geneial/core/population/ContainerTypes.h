#pragma once

#include <geneial/namespaces.h>

#include <vector>


geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_export_namespace
{
template<typename FITNESS_TYPE>
class ContainerTypes
{
private:
    ContainerTypes() = delete;
    ContainerTypes(const ContainerTypes& type) = delete;
    ContainerTypes(const ContainerTypes&& type) = delete;

public:
    //A generic container to pass chromomsomes between operations.
    typedef typename std::vector<typename chromosome::BaseChromosome<FITNESS_TYPE>::ptr> chromosome_container;
};

} /* geneial_export_namespace */
} /* private namespace population */
} /* private namespace geneial */
