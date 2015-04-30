#pragma once

#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/utility/Random.h>

#include <cassert>

namespace geneial
{
namespace operation
{
namespace mutation
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class NonUniformMutationOperation: public BaseMutationOperation<FITNESS_TYPE>
{

private:
    const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &_builderSettings;
    MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> &_builderFactory;

    double _minimumModification;
    unsigned int _affectedGenerations;
public:
    /*
     * NonUniformMutationOperation Mutates a chromosome, by modificating some of it's values with weight w.
     * W decreases with the amount of mutations defined by affectedGenerations.
     * minimumModification defines the percetual change to a value AFTER the number of affected Generations has been reached
     *
     * @param affectedGenerations defines how many Generations will be target for decreasing amounts of mutation
     * @param minimumModification defines the lowest percentage of mutation that is possible
     */

    NonUniformMutationOperation(unsigned int affectedGenerations, double minimumModification,
            const MutationSettings &settings, const BaseChoosingOperation<FITNESS_TYPE> &choosingOperation,
            const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &builderSettings,
            MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> &builderFactory) :
            BaseMutationOperation<FITNESS_TYPE>(settings, choosingOperation), _builderSettings(builderSettings), _builderFactory(
                    builderFactory), _minimumModification(minimumModification), _affectedGenerations(
                    affectedGenerations)
    {
    }

    virtual ~NonUniformMutationOperation()
    {
    }

    /*
     *  Returns a new chromosome which is a partially mutated version of the old one.
     *  */
    virtual typename Population<FITNESS_TYPE>::chromosome_container doMutate(
            const typename Population<FITNESS_TYPE>::chromosome_container &mutants, BaseManager<FITNESS_TYPE> &manager) override;

    MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> & getBuilderFactory() const
    {
        return _builderFactory;
    }

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
//class

} /* namespace mutation */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/mutation/NonUniformMutationOperation.hpp>
