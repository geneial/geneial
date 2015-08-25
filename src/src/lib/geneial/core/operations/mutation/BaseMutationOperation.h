#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/core/population/Population.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>
#include <geneial/core/operations/choosing/ChooseRandom.h>
#include <geneial/utility/mixins/Buildable.h>

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
using ::geneial::utility::Buildable;

geneial_export_namespace
{
template<typename FITNESS_TYPE>
class BaseMutationOperation : public Buildable<BaseMutationOperation<FITNESS_TYPE>>
{
protected:
    std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> _choosingOperation;

    //Constructor
    BaseMutationOperation(
            const std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation
            ) :
            _choosingOperation(choosingOperation)
    {
    }

public:
    //Destructor
    virtual ~BaseMutationOperation()
    {
    }

    typedef typename Population<FITNESS_TYPE>::chromosome_container mutation_result_set;
    virtual typename Population<FITNESS_TYPE>::chromosome_container doMutate(
            const typename Population<FITNESS_TYPE>::chromosome_container &mutants, BaseManager<FITNESS_TYPE> &manager) const = 0;

    BaseChoosingOperation<FITNESS_TYPE> const & getChoosingOperation() const
    {
        return *_choosingOperation;
    }

    class Builder : public Buildable<BaseMutationOperation<FITNESS_TYPE>>::Builder
    {

    protected:
        std::shared_ptr<BaseChoosingOperation<FITNESS_TYPE>> _choosingOperation;

    public:
        Builder() :
                _choosingOperation(typename ChooseRandom<FITNESS_TYPE>::Builder().create())
        {
        }

        Builder(const std::shared_ptr<BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation) :
                _choosingOperation(choosingOperation)
        {
        }

        Builder& setChoosingOperation(const std::shared_ptr<BaseChoosingOperation<FITNESS_TYPE> >& choosingOperation)
        {
            _choosingOperation = choosingOperation;
            return *this;
        }
    };

};

} /* geneial_export_namespace */
} /* private namespace mutation */
} /* private namespace operation */
} /* private namespace geneial */


