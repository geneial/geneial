#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{
geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class ContinousMultiValueChromosomeFactory: public MultiValueChromosomeFactory<VALUE_TYPE,FITNESS_TYPE>
{
private:
    const ContinousMultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &_settings;

public:
    explicit ContinousMultiValueChromosomeFactory(const ContinousMultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &settings) :
        MultiValueChromosomeFactory<VALUE_TYPE,FITNESS_TYPE>(settings), _settings(settings)
    {
    }

protected:
    typename BaseChromosome<FITNESS_TYPE>::ptr doCreateChromosome(
            typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues,
            BaseManager<FITNESS_TYPE>& manager
        ) override;
};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */

#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.hpp>

