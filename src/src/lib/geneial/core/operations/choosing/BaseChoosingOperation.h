#pragma once

#include <geneial/namespaces.h>

#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/population/Population.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(choosing)
{
using ::geneial::population::Population;

geneial_export_namespace
{

template<typename FITNESS_TYPE>
class BaseChoosingOperation
{
public:
    BaseChoosingOperation()
    {
    }

    virtual ~BaseChoosingOperation()
    {
    }

    virtual typename Population<FITNESS_TYPE>::chromosome_container doChoose(
            const typename Population<FITNESS_TYPE>::chromosome_container &chromosomeInputSet) const = 0;
};

} /* geneial_export_namespace */
} /* private namespace choosing */
} /* private namespace operation */
} /* private namespace geneial */
