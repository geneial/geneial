#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{
geneial_export_namespace
{

template<typename FITNESS_TYPE>
class BaseChromosomeFactory
{
private:
public:

    enum PopulateBehavior
    {
        CREATE_VALUES, LET_UNPOPULATED
    };

    BaseChromosomeFactory()
    {
    }

    virtual ~BaseChromosomeFactory()
    {
    }

    virtual typename BaseChromosome<FITNESS_TYPE>::ptr createChromosome(
            PopulateBehavior populateValues = CREATE_VALUES) = 0;
};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */
