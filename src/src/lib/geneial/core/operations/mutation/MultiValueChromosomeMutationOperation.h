#pragma once

#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/core/operations/mutation/MultiValueMutationSettings.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(mutation)
{
using ::geneial::population::chromosome::MultiValueChromosome;
using ::geneial::population::chromosome::MultiValueChromosomeFactory;

geneial_export_namespace
{
//The purpose of this class is to provide heirs the MVCFactory and to encapsulate high level mvc specific operations
template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeMutationOperation : public BaseMutationOperation<FITNESS_TYPE>
{

protected:
    std::shared_ptr<const MultiValueMutationSettings> _settings;

    std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> _builderFactory;

    MultiValueChromosomeMutationOperation(
            const std::shared_ptr<const MultiValueMutationSettings> &settings,
            const std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation,
            const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory
            ) :
                BaseMutationOperation<FITNESS_TYPE>(choosingOperation),
                _settings(settings),
                _builderFactory(builderFactory)
    {
    }

public:

    virtual ~MultiValueChromosomeMutationOperation()
    {
    }

    inline MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> & getBuilderFactory() const
    {
        return *_builderFactory;
    }

    inline const MultiValueMutationSettings& getSettings() const
    {
        return *_settings;
    }

    class Builder : public BaseMutationOperation<FITNESS_TYPE>::Builder
    {
    protected:
        std::shared_ptr<MultiValueMutationSettings> _settings;
        std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> _builderFactory;

    public:
        Builder(const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory):
                    BaseMutationOperation<FITNESS_TYPE>::Builder(),
                    _settings(std::make_shared<MultiValueMutationSettings>()),
                    _builderFactory(builderFactory)
                {}

        Builder(
                const std::shared_ptr<MultiValueMutationSettings> &settings,
                const std::shared_ptr<BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation,
                const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory):
                    BaseMutationOperation<FITNESS_TYPE>::Builder(choosingOperation),
                    _settings(settings),
                    _builderFactory(builderFactory)
                {}

        const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> >& getBuilderFactory() const
        {
            return _builderFactory;
        }

        void setBuilderFactory(
                const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> >& builderFactory)
        {
            _builderFactory = builderFactory;
        }

        MultiValueMutationSettings& getSettings()
        {
            return *_settings;
        }

        void setSettings(const std::shared_ptr<MultiValueMutationSettings>& settings)
        {
            _settings = settings;
        }
    };

};

} /* geneial_export_namespace */
} /* private namespace mutation */
} /* private namespace operation */
} /* private namespace geneial */

