#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/core/population/Population.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>
#include <geneial/core/operations/choosing/ChooseRandom.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(mutation)
{
using ::geneial::operation::choosing::BaseChoosingOperation;
using ::geneial::operation::choosing::ChooseRandom;
using ::geneial::population::Population;
using ::geneial::population::management::BaseManager;

geneial_export_namespace
{
template<typename FITNESS_TYPE>
class BaseMutationOperation
{
protected:


    std::shared_ptr<const MutationSettings>_settings;

    std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> _choosingOperation;

    //Constructor
    BaseMutationOperation(
            const std::shared_ptr<const MutationSettings> &settings,
            const std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation
            ) :
            _settings(settings), _choosingOperation(choosingOperation)
    {
    }

public:
    using ptr = std::shared_ptr<BaseMutationOperation<FITNESS_TYPE>>;

    using const_ptr = std::shared_ptr<BaseMutationOperation<FITNESS_TYPE>>;


    //Destructor
    virtual ~BaseMutationOperation()
    {
    }

    typedef typename Population<FITNESS_TYPE>::chromosome_container mutation_result_set;
    virtual typename Population<FITNESS_TYPE>::chromosome_container doMutate(
            const typename Population<FITNESS_TYPE>::chromosome_container &mutants, BaseManager<FITNESS_TYPE> &manager) const = 0;

    MutationSettings const & getSettings() const
    {
        return *_settings;
    }

    void setSettings(const std::shared_ptr<const MutationSettings> &settings)
    {
        _settings = settings;
    }

    BaseChoosingOperation<FITNESS_TYPE> const & getChoosingOperation() const
    {
        return *_choosingOperation;
    }

    class Builder
    {

    protected:
        std::shared_ptr<MutationSettings> _settings;

        std::shared_ptr<BaseChoosingOperation<FITNESS_TYPE>> _choosingOperation;

    public:

        static const constexpr     double DEFAULT_PROBABILITY= 0.1;

        static const constexpr double DEFAULT_AMOUNT_OF_MUTATION = 0.1;

        static const unsigned int DEFAULT_AMOUNT_OF_POINTS = 5;

        Builder():
            _settings(new MutationSettings(DEFAULT_PROBABILITY,DEFAULT_AMOUNT_OF_MUTATION,DEFAULT_AMOUNT_OF_POINTS)),
            _choosingOperation(ChooseRandom<FITNESS_TYPE>::Builder(DEFAULT_PROBABILITY).create())
        {
        }

        Builder(const std::shared_ptr<MutationSettings> &settings,
                const std::shared_ptr<BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation):
                    _settings(settings),
                    _choosingOperation(choosingOperation)
        {
        }

        virtual ptr create() = 0;

        void setChoosingOperation(const std::shared_ptr<BaseChoosingOperation<FITNESS_TYPE> >& choosingOperation)
        {
            _choosingOperation = choosingOperation;
        }

        inline MutationSettings& getSettings() const
        {
            return *_settings;
        }

        void setSettings(const std::shared_ptr<MutationSettings>& settings)
        {
            _settings = settings;
        }
    };

};

} /* geneial_export_namespace */
} /* private namespace mutation */
} /* private namespace operation */
} /* private namespace geneial */


