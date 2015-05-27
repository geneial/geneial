#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(choosing)
{
using ::geneial::population::Population;
using ::geneial::operation::mutation::MutationSettings;

geneial_export_namespace
{


template<typename FITNESS_TYPE>
class ChooseRandom: public BaseChoosingOperation<FITNESS_TYPE>
{
private:
    double _probability;

protected:
    explicit ChooseRandom(const double probability) :
            BaseChoosingOperation<FITNESS_TYPE>(), _probability(probability)
    {
    }

public:

    virtual ~ChooseRandom()
    {
    }

    /**
     * Chooses from an chromosomeContainer Random values. The propability that a Chromosome is choosen and therefore part of the
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

        const static double DEFAULT_PROBABILITY = 0.1;

        double _probability;

    public:

        Builder() :
                _probability(DEFAULT_PROBABILITY)
        {
        }

        Builder(const double _probability) :
                _probability(probability)
        {
        }

        virtual typename BaseChoosingOperation<FITNESS_TYPE>::ptr create() override
        {
            typename BaseChoosingOperation<FITNESS_TYPE>::ptr prototype(new ChooseRandom<FITNESS_TYPE>(this->_probability));
            return std::move(prototype);
        }

        double getProbability() const
        {
            return _probability;
        }

        void setProbability(double probability = 0.1)
        {
            _probability = probability;
        }
    };

};

} /* geneial_export_namespace */
} /* private namespace choosing */
} /* private namespace operation */
} /* private namespace geneial */


#include <geneial/core/operations/choosing/ChooseRandom.hpp>

