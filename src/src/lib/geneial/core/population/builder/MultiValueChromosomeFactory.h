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
        /*for(auto & ptr : _usePool)
                {
        //TODO
                    delete ptr;
                }*/
        for(auto & ptr : _freePool)
                {
            //TODO
                    delete ptr;
                }
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

    std::vector<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>*> _usePool;
    std::vector<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>*> _freePool;

    friend class FactoryDeleter;

    struct FactoryDeleter {
        MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> *_factory;
        FactoryDeleter(MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> *factory):_factory(factory)
        {
        }

        void operator()(MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>* p) const {
            _factory->free(p);
        }
    };

    void free(MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>* p)
    {
        if(_freePool.size()<100)
        {
            _usePool.erase(std::find(_usePool.begin(),_usePool.end(),p));
            _freePool.push_back(p);
        }
        else
        {
            //TODO
            delete p;
        }
    }


    typename MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE>::ptr allocateNewChromsome()
    {
        MultiValueChromosome<VALUE_TYPE,FITNESS_TYPE> *rawChromosome ;

        if (_freePool.size() > 0)
        {
            rawChromosome = _freePool.back();
            _freePool.pop_back();
            _usePool.push_back(rawChromosome);
            rawChromosome->setFitnessEvaluator(_settings.getFitnessEvaluator());
            rawChromosome->invalidate();
        }
        else
        {
            rawChromosome = new MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>(_settings.getFitnessEvaluator());
            _usePool.push_back(rawChromosome);
        }


        typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr new_chromosome(rawChromosome, FactoryDeleter(this));

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
