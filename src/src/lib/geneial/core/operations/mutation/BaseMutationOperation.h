#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/core/population/Population.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(mutation)
{
using ::geneial::operation::choosing::BaseChoosingOperation;
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

public:
    //Constructor
    BaseMutationOperation(
            const std::shared_ptr<const MutationSettings> &settings,
            const std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation
            ) :
            _settings(settings), _choosingOperation(choosingOperation)
    {
    }

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
};

} /* geneial_export_namespace */
} /* private namespace mutation */
} /* private namespace operation */
} /* private namespace geneial */


