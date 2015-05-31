#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/builder/BaseChromosomeFactory.h>
#include <geneial/core/population/builder/MultiValueBuilderSettings.h>

#include <memory>

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

    //Note that the raw ptr is intended here, since we wish to reduce the overhead.
    std::shared_ptr<ResourcePool<MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>>>  _chromosomeResourcePool;


public:
    explicit MultiValueChromosomeFactory(const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &settings) :
        BaseChromosomeFactory<FITNESS_TYPE>(),
        _settings(settings),
        _chromosomeResourcePool(std::make_shared<ResourcePool<MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>>>())
    {
    }

    virtual ~MultiValueChromosomeFactory()
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


    friend class FactoryDeleter;

    struct ChromosomeDeleter {
        std::weak_ptr<ResourcePool<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>>>  _resourcePool;

        ChromosomeDeleter(std::weak_ptr<ResourcePool<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>>> resourcePool):_resourcePool(resourcePool)
        {
        }

        inline void operator()(MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>* p) const
        {
            if (const auto resourcePool = _resourcePool.lock())
            {
                resourcePool->free(std::move(p));
            }
            else
            {
                delete p;
            }
        }
    };


    typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr allocateNewChromsome()
    {
        MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> *rawChromosome ;

        if(this->_chromosomeResourcePool->retrieve(rawChromosome))
        {
            rawChromosome->invalidate();
            rawChromosome->setFitnessEvaluator(_settings.getFitnessEvaluator());
        }
        else
        {
            rawChromosome = new MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>(_settings.getFitnessEvaluator());
        }


        typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr new_chromosome(
                rawChromosome, ChromosomeDeleter(this->_chromosomeResourcePool));

        new_chromosome->getContainer().resize(this->_settings.getNum());

        return std::move(new_chromosome);
    }
};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */

#include <geneial/core/population/builder/MultiValueChromosomeFactory.hpp>
