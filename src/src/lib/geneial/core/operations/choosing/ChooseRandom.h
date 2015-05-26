#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(choosing)
{
using ::geneial::population::Population;
using ::geneial::operation::mutation::MutationSettings;

geneial_export_namespace
{


template<typename VALUE_TYPE, typename FITNESS_TYPE>
class ChooseRandom: public BaseChoosingOperation<FITNESS_TYPE>
{
private:
    std::shared_ptr<const MutationSettings> _settings;

public:
    explicit ChooseRandom(const std::shared_ptr<const MutationSettings> &settings) :
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

    inline const MutationSettings& getSettings() const
    {
        return *_settings;
    }

    void setSettings(const std::shared_ptr<const MutationSettings> &settings)
    {
        _settings = settings;
    }
};

} /* geneial_export_namespace */
} /* private namespace choosing */
} /* private namespace operation */
} /* private namespace geneial */


#include <geneial/core/operations/choosing/ChooseRandom.hpp>

