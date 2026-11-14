#pragma once

#include <geneial/core/population/builder/BaseChromosomeFactory.h>
#include <geneial/core/population/builder/MultiValueBuilderSettings.h>
#include <geneial/namespaces.h>
#include <geneial/utility/mixins/EnableMakeShared.h>

#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <geneial/core/population/management/ResourcePool.h>
#include <memory>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(population)
    {
        geneial_private_namespace(chromosome)
        {

            using ::geneial::population::management::ResourcePool;
            using ::geneial::utility::EnableMakeShared;

            geneial_export_namespace
            {

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                class MultiValueChromosomeFactory
                    : public BaseChromosomeFactory<FITNESS_TYPE>,
                      public virtual EnableMakeShared<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>>
                {
                  protected:
                    struct ChromosomeDeleter
                    {
                        std::weak_ptr<ResourcePool<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>>> _resourcePool;

                        ChromosomeDeleter(
                            const std::weak_ptr<ResourcePool<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>>>
                                &resourcePool)
                            : _resourcePool(resourcePool)
                        {
                        }

                        inline void operator()(MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> *p) const
                        {
                            p->invalidate();
                            p->setFitnessEvaluator(nullptr);
                            if (const auto resourcePool = _resourcePool.lock())
                            {
                                resourcePool->free(std::move(p));
                            }
                            else
                            {
                                // The resource pool is already gone, manual fallback resource cleanup.
                                delete p;
                            }
                        }
                    };

                    std::shared_ptr<MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>> _settingsOwner;
                    MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &_settings;

                    /** Cached resources belong to the factory; outstanding chromosomes hold only a weak pool link. */
                    std::shared_ptr<ResourcePool<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>>>
                        _chromosomeResourcePool;

                    typename BaseChromosome<FITNESS_TYPE>::ptr doCreateChromosome(
                        typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues) override;

                    typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr allocateNewChromsome()
                    {
                        MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> *rawChromosome;

                        if (this->_chromosomeResourcePool->retrieve(rawChromosome))
                        {
                            rawChromosome->invalidate();
                            rawChromosome->setFitnessEvaluator(_settings.getFitnessEvaluator());
                        }
                        else
                        {
                            rawChromosome =
                                new MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>(_settings.getFitnessEvaluator());
                        }

                        typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr new_chromosome(
                            rawChromosome, ChromosomeDeleter(this->_chromosomeResourcePool));
                        new_chromosome->setAge(0);

                        // Enforce alloc
                        new_chromosome->getContainer().resize(this->_settings.getNum());

                        return new_chromosome;
                    }

                    explicit MultiValueChromosomeFactory(
                        const std::shared_ptr<MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>> &settings)
                        : BaseChromosomeFactory<FITNESS_TYPE>(), _settingsOwner(settings), _settings(*settings),
                          _chromosomeResourcePool(
                              std::make_shared<ResourcePool<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>>>())
                    {
                    }

                  public:
                    class Builder : public BaseChromosomeFactory<FITNESS_TYPE>::Builder
                    {
                        std::shared_ptr<MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>> _settings;

                      public:
                        MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &getSettings()
                        {
                            return *_settings;
                        }

                        Builder(const typename FitnessEvaluator<FITNESS_TYPE>::ptr &fitnessEvaluator)
                            : _settings(std::make_shared<MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>>(
                                  fitnessEvaluator))
                        {
                        }

                        virtual typename BaseChromosomeFactory<FITNESS_TYPE>::ptr create() override
                        {
                            return std::move(
                                MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>::makeShared(_settings));
                        }
                    };

                    virtual ~MultiValueChromosomeFactory() {}

                    std::size_t getCachedChromosomeCount() const
                    {
                        return _chromosomeResourcePool->size();
                    }
                    std::size_t getResourcePoolCapacity() const
                    {
                        return _chromosomeResourcePool->capacity();
                    }
                    void setResourcePoolCapacity(std::size_t capacity)
                    {
                        _chromosomeResourcePool->setCapacity(capacity);
                    }
                    void clearResourcePool()
                    {
                        _chromosomeResourcePool->clear();
                    }

                    inline const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &getSettings() const
                    {
                        return _settings;
                    }

                    void setSettings(const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &settings)
                    {
                        _settings = settings;
                    }
                };

            } /* geneial_export_namespace */
        } /* private namespace chromosome */
    } /* private namespace population */
} /* private namespace geneial */

#include <geneial/core/population/builder/MultiValueChromosomeFactory.hpp>
