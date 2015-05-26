#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/core/population/management/BaseManager.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{
using ::geneial::population::management::BaseManager;

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

    inline typename BaseChromosome<FITNESS_TYPE>::ptr createChromosome(
            PopulateBehavior populateValues = CREATE_VALUES
        )
    {
        return std::move(doCreateChromosome(populateValues));
    }

protected:
    virtual typename BaseChromosome<FITNESS_TYPE>::ptr doCreateChromosome(
            PopulateBehavior populateValues
        ) = 0;
};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */
