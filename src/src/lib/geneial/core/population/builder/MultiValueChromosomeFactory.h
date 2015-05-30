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
class ResourcePool
{
public:
    void free(MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>* p)
    {
        _usePool.erase(std::find(_usePool.begin(),_usePool.end(),p));
        _freePool.emplace_back(p);
    }
    std::vector<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>*> _usePool;
    std::vector<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>*> _freePool;
    virtual ~ResourcePool()
    {
        //The used pool is out in the wild guarded by sharedptr.
        for (auto & ptr : _freePool)
        {
            delete ptr;
        }
        for (auto & ptr : _usePool)
        {
            delete ptr;
        }
    }
};

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeFactory: public BaseChromosomeFactory<FITNESS_TYPE>
{
protected:
    const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &_settings;
    std::shared_ptr<ResourcePool<VALUE_TYPE, FITNESS_TYPE>> _resourcePool;


public:
    explicit MultiValueChromosomeFactory(const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &settings) :
        BaseChromosomeFactory<FITNESS_TYPE>(),
        _settings(settings),
        _resourcePool(std::make_shared<ResourcePool<VALUE_TYPE, FITNESS_TYPE>>())
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

    struct FactoryDeleter {

        std::shared_ptr<ResourcePool<VALUE_TYPE, FITNESS_TYPE>> _resourcePool;

        FactoryDeleter(std::shared_ptr<ResourcePool<VALUE_TYPE, FITNESS_TYPE>>resourcePool):_resourcePool(resourcePool)
        {
        }

        void operator()(MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>* p) const
        {
            _resourcePool->free(p);
        }
    };



    typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr allocateNewChromsome()
    {
        MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> *rawChromosome ;
        if (_resourcePool->_freePool.size() > 0)
        {
            rawChromosome = _resourcePool->_freePool.back();
            _resourcePool->_freePool.pop_back();
            _resourcePool->_usePool.push_back(rawChromosome);
            rawChromosome->invalidate();
            rawChromosome->setFitnessEvaluator(_settings.getFitnessEvaluator());
        }
        else
        {
            rawChromosome = new MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>(_settings.getFitnessEvaluator());
            _resourcePool->_usePool.push_back(rawChromosome);
        }


        typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr new_chromosome(rawChromosome, FactoryDeleter(_resourcePool));

        auto manager = this->getManager();

        if (!manager.expired())
        {
            auto _manager = manager.lock();
            //assert(!_manager->getPopulation().hashExists(new_chromosome->getHash()) && "Consistency Problem, HoldoffSet contains Population member!");
        }

/*
        if (!new_chromosome)
        {
            _allocated++;
            new_chromosome = std::make_shared<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>>(_settings.getFitnessEvaluator());
        }
*/

        new_chromosome->getContainer().resize(this->_settings.getNum());

        return new_chromosome;
    }
};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */

#include <geneial/core/population/builder/MultiValueChromosomeFactory.hpp>
