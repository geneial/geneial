#pragma once

#include <geneial/core/operations/mutation/MultiValueChromosomeMutationOperation.h>
#include <geneial/core/population/Population.h>

#include <cassert>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(mutation)
{
using ::geneial::population::Population;

geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class UniformMutationOperation: public MultiValueChromosomeMutationOperation<VALUE_TYPE,FITNESS_TYPE>
{
    /*
     * UniformMutationOperation Mutates a chromosome, by replacing some of it's values randomly.
     */
    UniformMutationOperation(
            const std::shared_ptr<const MutationSettings> &settings,
            const std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation,
            const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory) :

                MultiValueChromosomeMutationOperation<VALUE_TYPE,FITNESS_TYPE>(
                        settings,
                        choosingOperation,
                        builderFactory
                        )
    {
    }
public:

    virtual ~UniformMutationOperation()
    {
    }

    /*
     *  Returns a new chromosome which is a partially mutated version of the old one.
     *  */
    virtual typename Population<FITNESS_TYPE>::chromosome_container doMutate(
            const typename Population<FITNESS_TYPE>::chromosome_container &mutants, BaseManager<FITNESS_TYPE> &manager) const override;


    class Builder : public MultiValueChromosomeMutationOperation<VALUE_TYPE,FITNESS_TYPE>::Builder
    {
    public:

        Builder(const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory) :
                MultiValueChromosomeMutationOperation<VALUE_TYPE, FITNESS_TYPE>::Builder(builderFactory)
        {
        }

        Builder(const std::shared_ptr<MutationSettings> &settings,
                const std::shared_ptr<BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation,
                const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory) :
                MultiValueChromosomeMutationOperation<VALUE_TYPE, FITNESS_TYPE>::Builder(settings, choosingOperation,
                        builderFactory)
        {
        }

        virtual typename BaseMutationOperation<FITNESS_TYPE>::ptr create() override
        {
            typename BaseMutationOperation<FITNESS_TYPE>::ptr protoype(
                    new UniformMutationOperation<VALUE_TYPE, FITNESS_TYPE>(this->_settings, this->_choosingOperation,
                            this->_builderFactory));
            return std::move(protoype);
        }
    };

};

} /* geneial_export_namespace */
} /* private namespace mutation */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/mutation/UniformMutationOperation.hpp>

