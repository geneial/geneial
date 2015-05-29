#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/builder/BaseChromosomeFactory.h>
#include <geneial/core/population/builder/MultiValueBuilderSettings.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{
geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeFactory: public BaseChromosomeFactory<FITNESS_TYPE>
{
protected:
    const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &_settings;

public:
    explicit MultiValueChromosomeFactory(const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &settings) :
        BaseChromosomeFactory<FITNESS_TYPE>(),
        _settings(settings)
    {
    }




    inline const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> & getSettings() const
    {
        return _settings;
    }

    void setSettings(const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>& settings)
    {
        _settings = settings;
    }
protected:
    typename BaseChromosome<FITNESS_TYPE>::ptr doCreateChromosome(
            typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues
    ) override;

    typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr allocateNewChromsome()
    {
        typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr new_chromosome;

        auto manager = this->getManager();

        if (!manager.expired())
        {
            auto _manager = manager.lock();
            new_chromosome = std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>>(_manager->retrieveFromHoldOff());
            if(new_chromosome)
            {
                new_chromosome->invalidate();
            }
        }

        if (!new_chromosome)
        {
            new_chromosome = std::make_shared<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>>(_settings.getFitnessEvaluator());
        }

        new_chromosome->getContainer().resize(this->_settings.getNum());

        return new_chromosome;
    }
};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */

#include <geneial/core/population/builder/MultiValueChromosomeFactory.hpp>
