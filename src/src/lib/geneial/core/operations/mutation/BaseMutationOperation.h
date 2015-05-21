#pragma once

#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/core/population/Population.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>

namespace geneial
{
namespace operation
{
namespace mutation
{

using namespace geneial::operation::choosing;
using namespace geneial::population::management;

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

} /* namespace mutation */
} /* namespace operation */
} /* namespace geneial */

