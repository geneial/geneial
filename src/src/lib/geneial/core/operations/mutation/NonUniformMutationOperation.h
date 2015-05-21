#pragma once

#include <geneial/core/operations/mutation/MultiValueChromosomeMutationOperation.h>
#include <geneial/utility/Random.h>

#include <cassert>

namespace geneial
{
namespace operation
{
namespace mutation
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class NonUniformMutationOperation: public MultiValueChromosomeMutationOperation<VALUE_TYPE,FITNESS_TYPE>
{

private:

    double _minimumModification;

    unsigned int _affectedGenerations;

public:
    /**
     * (TODO) bewo outdated docs.
     *
     * NonUniformMutationOperation Mutates a chromosome, by modificating some of it's values with weight w.
     * W decreases with the amount of mutations defined by affectedGenerations.
     * minimumModification defines the percetual change to a value AFTER the number of affected Generations has been reached
     *
     * @param affectedGenerations defines how many Generations will be target for decreasing amounts of mutation
     * @param minimumModification defines the lowest percentage of mutation that is possible
     */
    NonUniformMutationOperation(
            const unsigned int affectedGenerations,
            const VALUE_TYPE minimumModification,
            const std::shared_ptr<const MutationSettings> &settings,
            const std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation,
            const std::shared_ptr<const MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory) :

                MultiValueChromosomeMutationOperation<FITNESS_TYPE,VALUE_TYPE>(
                        settings,
                        choosingOperation,
                        builderFactory
                        ),

                _minimumModification(minimumModification),
                _affectedGenerations(affectedGenerations)
    {
    }

    virtual ~NonUniformMutationOperation()
    {
    }

    /**
     *  Returns a new chromosome which is a partially mutated version of the old one.
     *  */
    virtual typename Population<FITNESS_TYPE>::chromosome_container doMutate(
            const typename Population<FITNESS_TYPE>::chromosome_container &mutants, BaseManager<FITNESS_TYPE> &manager) const override;


    double getMinimumModification() const
    {
        return _minimumModification;
    }

    void setMinimumModification(double minimumModification)
    {
        _minimumModification = minimumModification;
    }

    unsigned int getAffectedGenerations() const
    {
        return _affectedGenerations;
    }

    void setAffectedGenerations(unsigned int affectedGenerations)
    {
        this->affectedGenerations = affectedGenerations;
    }
};

} /* namespace mutation */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/mutation/NonUniformMutationOperation.hpp>
