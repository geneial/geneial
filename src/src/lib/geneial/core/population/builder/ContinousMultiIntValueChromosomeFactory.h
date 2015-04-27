#pragma once

#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/core/population/builder/MultiIntValueChromosomeFactory.h>
#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>

namespace geneial
{
namespace population
{
namespace chromosome
{

template<typename FITNESS_TYPE>
class ContinousMultiIntValueChromosomeFactory: public MultiIntValueChromosomeFactory<FITNESS_TYPE>
{
private:
    ContinousMultiValueBuilderSettings<int, FITNESS_TYPE> *_settings;
public:
    ContinousMultiIntValueChromosomeFactory(ContinousMultiValueBuilderSettings<int, FITNESS_TYPE> *settings) :
            MultiIntValueChromosomeFactory<FITNESS_TYPE>(settings), _settings(settings)
    {
    }

    typename BaseChromosome<FITNESS_TYPE>::ptr createChromosome(
            typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues = BaseChromosomeFactory<
                    FITNESS_TYPE>::CREATE_VALUES);
};

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

#include <geneial/core/population/builder/ContinousMultiIntValueChromosomeFactory.hpp>

