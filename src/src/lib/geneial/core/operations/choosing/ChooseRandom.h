#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>

#include <geneial/utility/mixins/Buildable.h>
#include <geneial/utility/mixins/EnableMakeShared.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(choosing)
{
using ::geneial::population::Population;
using ::geneial::utility::EnableMakeShared;

geneial_export_namespace
{


template<typename FITNESS_TYPE>
class ChooseRandom: public BaseChoosingOperation<FITNESS_TYPE>,
                    public EnableMakeShared<ChooseRandom<FITNESS_TYPE>>
{
private:
    double _probability;
protected:
    explicit ChooseRandom(const double probability=DEFAULT_PROBABILITY) :
            BaseChoosingOperation<FITNESS_TYPE>(), _probability(probability)
    {
    }

public:
    const constexpr static double DEFAULT_PROBABILITY = 0.1;

    virtual ~ChooseRandom()
    {
    }

    /**
     * Chooses from an chromosomeContainer Random values. The probability that a Chromosome is choosen and therefore part of the
     * resultset is defined in MutationSettings.
     */
    virtual typename Population<FITNESS_TYPE>::chromosome_container doChoose(
            const typename Population<FITNESS_TYPE>::chromosome_container &chromosomeInputSet) const override;

    double getProbability() const
    {
        return _probability;
    }

    void setProbability(double probability)
    {
        _probability = probability;
    }

    class Builder: public BaseChoosingOperation<FITNESS_TYPE>::Builder
    {

    protected:
        double _probability;

    public:
        const constexpr static double DEFAULT_PROBABILITY = 0.1;

        Builder() : BaseChoosingOperation<FITNESS_TYPE>::Builder(),
                _probability(DEFAULT_PROBABILITY)
        {
        }

        Builder(const double probability) :
                BaseChoosingOperation<FITNESS_TYPE>::Builder(),
                _probability(probability)
        {
        }

        virtual typename BaseChoosingOperation<FITNESS_TYPE>::ptr create() override
        {
            return ChooseRandom<FITNESS_TYPE>::makeShared(this->_probability);
        }

        double getProbability() const
        {
            return _probability;
        }

        Builder& setProbability(double probability = DEFAULT_PROBABILITY)
        {
            _probability = probability;
            return *this;
        }
    };

};

} /* geneial_export_namespace */
} /* private namespace choosing */
} /* private namespace operation */
} /* private namespace geneial */


#include <geneial/core/operations/choosing/ChooseRandom.hpp>

