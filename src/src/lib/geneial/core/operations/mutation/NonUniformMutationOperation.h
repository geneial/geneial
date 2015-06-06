#pragma once

#include <geneial/core/operations/mutation/MultiValueChromosomeMutationOperation.h>
#include <geneial/core/population/Population.h>
#include <geneial/utility/mixins/EnableMakeShared.h>

#include <geneial/utility/Random.h>

#include <cassert>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(mutation)
{
using ::geneial::population::Population;
using ::geneial::utility::EnableMakeShared;

geneial_export_namespace
{
template<typename VALUE_TYPE, typename FITNESS_TYPE>
class NonUniformMutationOperation:  public MultiValueChromosomeMutationOperation<VALUE_TYPE,FITNESS_TYPE>,
                                    public EnableMakeShared<NonUniformMutationOperation<VALUE_TYPE,FITNESS_TYPE>>
{

private:

    double _minimumModification;

    unsigned int _affectedGenerations;

protected:
    NonUniformMutationOperation(
            const unsigned int affectedGenerations,
            const VALUE_TYPE minimumModification,
            const std::shared_ptr<const MutationSettings> &settings,
            const std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation,
            const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory) :

                MultiValueChromosomeMutationOperation<VALUE_TYPE,FITNESS_TYPE>(
                        settings,
                        choosingOperation,
                        builderFactory
                        ),

                _minimumModification(minimumModification),
                _affectedGenerations(affectedGenerations)
    {
    }

public:
    /**
     * TODO (bewo):  outdated docs.
     *
     * NonUniformMutationOperation Mutates a chromosome, by modificating some of it's values with weight w.
     * W decreases with the amount of mutations defined by affectedGenerations.
     * minimumModification defines the percetual change to a value AFTER the number of affected Generations has been reached
     *
     * @param affectedGenerations defines how many Generations will be target for decreasing amounts of mutation
     * @param minimumModification defines the lowest percentage of mutation that is possible
     */

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
        this->_affectedGenerations = affectedGenerations;
    }


    class Builder : public MultiValueChromosomeMutationOperation<VALUE_TYPE,FITNESS_TYPE>::Builder
    {
    protected:
        double _minimumModification;

        unsigned int _affectedGenerations;

    public:

        const constexpr static double DEFAULT_MINIMUM_MODIFICATION = 0.2;
        const constexpr static unsigned int DEFAULT_AFFECTED_GENERATIONS = 1000;

        Builder(const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory) :
                MultiValueChromosomeMutationOperation<VALUE_TYPE, FITNESS_TYPE>::Builder(builderFactory), _minimumModification(
                        DEFAULT_MINIMUM_MODIFICATION), _affectedGenerations(DEFAULT_AFFECTED_GENERATIONS)
        {
        }

        Builder(const std::shared_ptr<MutationSettings> &settings,
                const std::shared_ptr<BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation,
                const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory) :
                MultiValueChromosomeMutationOperation<VALUE_TYPE, FITNESS_TYPE>::Builder(settings, choosingOperation,
                        builderFactory), _minimumModification(DEFAULT_MINIMUM_MODIFICATION), _affectedGenerations(
                        DEFAULT_AFFECTED_GENERATIONS)
        {
        }

        Builder(const double minimumModification,
                const unsigned int affectedGenerations,
                const std::shared_ptr<MutationSettings> &settings,
                const std::shared_ptr<BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation,
                const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory) :
                MultiValueChromosomeMutationOperation<VALUE_TYPE, FITNESS_TYPE>::Builder(settings, choosingOperation,
                        builderFactory), _minimumModification(minimumModification), _affectedGenerations(
                        affectedGenerations)
        {
        }

        virtual typename BaseMutationOperation<FITNESS_TYPE>::ptr create() override
        {
            return std::move
                    (
                        NonUniformMutationOperation<VALUE_TYPE, FITNESS_TYPE>::makeShared
                        (
                            _affectedGenerations,
                            _minimumModification,
                            this->_settings,
                            this->_choosingOperation,
                            this->_builderFactory
                        )
                    );
        }

        Builder& setAffectedGenerations(unsigned int affectedGenerations)
        {
            _affectedGenerations = affectedGenerations;
            return *this;
        }

        Builder& setMinimumModification(double minimumModification)
        {
            _minimumModification = minimumModification;
            return *this;
        }
    };
};

} /* geneial_export_namespace */
} /* private namespace mutation */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/mutation/NonUniformMutationOperation.hpp>
