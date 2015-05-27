#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/coupling/CouplingSettings.h>
#include <geneial/core/operations/selection/BaseSelectionOperation.h>
#include <geneial/core/operations/crossover/BaseCrossoverOperation.h>
#include <geneial/core/population/management/BaseManager.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(coupling)
{
using ::geneial::population::Population;
using ::geneial::operation::crossover::BaseCrossoverOperation;
using ::geneial::operation::selection::BaseSelectionOperation;
using ::geneial::population::management::BaseManager;

geneial_export_namespace
{


template<typename FITNESS_TYPE>
class BaseCouplingOperation
{

private:
    std::shared_ptr<const CouplingSettings> _settings;

protected:
    explicit BaseCouplingOperation(const std::shared_ptr<const CouplingSettings> &settings) :
            _settings(settings)
    {
    }

public:
    using offspring_result_set = typename Population<FITNESS_TYPE>::chromosome_container;

    using ptr = std::shared_ptr<BaseCrossoverOperation<FITNESS_TYPE>>;

    using const_ptr = std::shared_ptr<BaseCrossoverOperation<FITNESS_TYPE>>;


    virtual ~BaseCouplingOperation()
    {
    }

    virtual offspring_result_set doCopulate(
            const typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &mating_pool,
            const BaseCrossoverOperation<FITNESS_TYPE> &crossoverOperation,
            BaseManager<FITNESS_TYPE> &manager) = 0;

    CouplingSettings const & getSettings() const
    {
        return *_settings;
    }

    void setSettings(const std::shared_ptr<const CouplingSettings> settings)
    {
        _settings = settings;
    }

    class Builder
    {

    protected:
        std::shared_ptr<CouplingSettings> _settings;

    public:
        const static unsigned int DEFAULT_NUMER_OF_OFFSPRINGS = 5;

        Builder():_settings(new CouplingSettings(DEFAULT_NUMER_OF_OFFSPRINGS))
        {
        }

        Builder(const std::shared_ptr<CouplingSettings> &settings):_settings(settings)
        {
        }

        virtual ptr create() = 0;

        inline CouplingSettings& getSettings() const
        {
            return *_settings;
        }

        void setSettings(const std::shared_ptr<const CouplingSettings>& settings)
        {
            _settings = settings;
        }

    };

protected:
   typename offspring_result_set::size_type copyUnlessMaximumReached(
            offspring_result_set &dest,
            const typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set &input,
            const typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set::size_type limit) const;

};

} /* geneial_export_namespace */
} /* private namespace coupling */
} /* private namespace operation */
} /* private namespace geneial */


#include <geneial/core/operations/coupling/BaseCouplingOperation.hpp>
