#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/fitness/FitnessEvaluator.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{
geneial_export_namespace
{

template<typename FITNESS_TYPE>
class BuilderSettings
{
private:
    typename FitnessEvaluator<FITNESS_TYPE>::ptr _fitnessEvaluator;

public:
    explicit BuilderSettings(const typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator) :
            _fitnessEvaluator(fitnessEvaluator)
    {
    }

    virtual ~BuilderSettings()
    {
    }

    const typename FitnessEvaluator<FITNESS_TYPE>::ptr& getFitnessEvaluator() const
    {
        return _fitnessEvaluator;
    }

    void setFitnessEvaluator(const typename FitnessEvaluator<FITNESS_TYPE>::ptr& fitnessEvaluator)
    {
        _fitnessEvaluator = fitnessEvaluator;
    }

};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */
