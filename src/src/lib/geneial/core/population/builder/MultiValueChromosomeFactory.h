#pragma once

#include <geneial/core/population/builder/BaseChromosomeFactory.h>
#include <geneial/core/population/builder/MultiValueBuilderSettings.h>

namespace geneial
{
namespace population
{
namespace chromosome
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeFactory: public BaseChromosomeFactory<FITNESS_TYPE>
{
protected:
    const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &_settings;

public:
    MultiValueChromosomeFactory(const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &settings) :
        BaseChromosomeFactory<FITNESS_TYPE>(),
        _settings(settings)
    {
    }

    typename BaseChromosome<FITNESS_TYPE>::ptr createChromosome(
            typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues = BaseChromosomeFactory<FITNESS_TYPE>::CREATE_VALUES) override;

    const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> & getSettings() const
    {
        return _settings;
    }

    void setSettings(const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>& settings)
    {
        _settings = settings;
    }
};

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

#include <geneial/core/population/builder/MultiValueChromosomeFactory.hpp>
