#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/replacement/BaseReplacementSettings.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/core/population/Population.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(replacement)
{
using ::geneial::population::Population;
using ::geneial::operation::crossover::BaseCrossoverOperation;
using ::geneial::operation::selection::BaseSelectionOperation;
using ::geneial::population::management::BaseManager;
using ::geneial::utility::Buildable;

geneial_export_namespace
{
template<typename FITNESS_TYPE>
class BaseReplacementOperation  : public Buildable<BaseReplacementOperation<FITNESS_TYPE>>
{
protected:
    std::shared_ptr<const BaseReplacementSettings> _settings;

public:
    using ptr = std::shared_ptr<BaseReplacementOperation<FITNESS_TYPE>>;

    using const_ptr = std::shared_ptr<BaseReplacementOperation<FITNESS_TYPE>>;

    explicit BaseReplacementOperation(const std::shared_ptr<const BaseReplacementSettings> &settings) :
            _settings(settings)
    {
    }

    virtual ~BaseReplacementOperation()
    {
    }

    virtual void doReplace(Population<FITNESS_TYPE> &population,
            const typename selection::BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &parents,
            typename coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring,
            BaseManager<FITNESS_TYPE> &manager) const = 0;

    const BaseReplacementSettings& getSettings() const
    {
        return *_settings;
    }

    void setSettings(const BaseReplacementSettings& settings)
    {
        _settings = settings;
    }

    class Builder : public Buildable<BaseReplacementOperation<FITNESS_TYPE>>::Builder
    {
    protected:
        std::shared_ptr<BaseReplacementSettings> _settings;

    public:
        Builder():_settings(new BaseReplacementSettings)
        {
        }

        BaseReplacementSettings& getSettings()
        {
            return *_settings;
        }

        void setSettings(const std::shared_ptr<BaseReplacementSettings>& settings)
        {
            _settings = settings;
        }
    };



};

} /* geneial_export_namespace */
} /* private namespace replacement */
} /* private namespace operation */
} /* private namespace geneial */

