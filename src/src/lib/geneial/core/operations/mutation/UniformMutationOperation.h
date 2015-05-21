#pragma once

#include <geneial/core/operations/mutation/MultiValueChromosomeMutationOperation.h>

#include <cassert>

namespace geneial
{
namespace operation
{
namespace mutation
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class UniformMutationOperation: public MultiValueChromosomeMutationOperation<VALUE_TYPE,FITNESS_TYPE>
{

private:
    const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &_builderSettings;
    MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> &_builderFactory;
public:
    /*
     * UniformMutationOperation Mutates a chromosome, by replacing some of it's values randomly.
     */
    UniformMutationOperation(
            const std::shared_ptr<const MutationSettings> &settings,
            const std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation,
            const std::shared_ptr<const MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory) :

                MultiValueChromosomeMutationOperation<FITNESS_TYPE,VALUE_TYPE>(
                        settings,
                        choosingOperation,
                        builderFactory
                        )
    {
    }


    /*
     *  Returns a new chromosome which is a partially mutated version of the old one.
     *  */
    virtual typename Population<FITNESS_TYPE>::chromosome_container doMutate(
            const typename Population<FITNESS_TYPE>::chromosome_container &mutants, BaseManager<FITNESS_TYPE> &manager) const override;


};

} /* namespace mutation */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/mutation/UniformMutationOperation.hpp>

