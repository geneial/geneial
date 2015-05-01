#pragma once

#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>

namespace geneial
{
namespace population
{
namespace chromosome
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

    typename BaseChromosome<FITNESS_TYPE>::ptr createChromosome(
            typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues = BaseChromosomeFactory<
                    FITNESS_TYPE>::CREATE_VALUES) override;
};

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.hpp>

