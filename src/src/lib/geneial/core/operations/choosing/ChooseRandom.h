#pragma once

#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>

namespace geneial
{
namespace operation
{
namespace choosing
{

using namespace geneial::population;
using namespace geneial::operation::mutation;

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class ChooseRandom: public BaseChoosingOperation<FITNESS_TYPE>
{
private:
    const MutationSettings &_settings;

public:
    ChooseRandom(const MutationSettings &settings) :
            BaseChoosingOperation<FITNESS_TYPE>(), _settings(settings)
    {
    }

    virtual ~ChooseRandom()
    {
    }

    /*
     * Chooses from an chromosomeContainer Random values. The propability that a Chromosome is choosen and therefore part of the
     * resultset is defined in MutationSettings.
     */
    virtual typename Population<FITNESS_TYPE>::chromosome_container doChoose(
            const typename Population<FITNESS_TYPE>::chromosome_container &chromosomeInputSet) const override;

    const MutationSettings& getSettings() const
    {
        return _settings;
    }

    void setSettings(const MutationSettings& settings)
    {
        _settings = settings;
    }
};

} /* namespace choosing */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/choosing/ChooseRandom.hpp>

