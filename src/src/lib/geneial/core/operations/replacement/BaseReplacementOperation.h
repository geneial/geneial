#pragma once

#include <geneial/core/operations/replacement/BaseReplacementSettings.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/core/population/Population.h>

namespace geneial
{
namespace operation
{
namespace replacement
{

using namespace geneial::operation::selection;
using namespace geneial::operation::coupling;
using namespace geneial::population::management;

template<typename FITNESS_TYPE>
class BaseReplacementOperation
{
protected:
    std::shared_ptr<const BaseReplacementSettings> _settings;

public:
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

};

} /* namespace replacement */
} /* namespace operation */
} /* namespace geneial */

