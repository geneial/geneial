#pragma once

#include <geneial/core/operations/mutation/BaseMutationOperation.h>

#include <cassert>

namespace geneial
{
namespace operation
{
namespace mutation
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class UniformMutationOperation: public BaseMutationOperation<FITNESS_TYPE>
{

private:
    const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &_builderSettings;
    MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> &_builderFactory;
public:
    /*
     * UniformMutationOperation Mutates a chromosome, by replacing some of it's values randomly.
     */
    UniformMutationOperation(const MutationSettings &settings, const BaseChoosingOperation<FITNESS_TYPE> &choosingOperation,
            const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &builderSettings,
            MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> &builderFactory) :
            BaseMutationOperation<FITNESS_TYPE>(settings, choosingOperation), _builderSettings(builderSettings), _builderFactory(
                    builderFactory)
    {
    }

    virtual ~UniformMutationOperation()
    {
    }

    /*
     *  Returns a new chromosome which is a partially mutated version of the old one.
     *  */
    virtual typename Population<FITNESS_TYPE>::chromosome_container doMutate(
            const typename Population<FITNESS_TYPE>::chromosome_container &mutants, BaseManager<FITNESS_TYPE> &manager) override;

    //TODO (lukas): copy paste from MultiValueChromosomeAvarageCrossover.h ...
    MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> & getBuilderFactory() const
    {
        return _builderFactory;
    }

};
//class

} /* namespace mutation */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/mutation/UniformMutationOperation.hpp>

