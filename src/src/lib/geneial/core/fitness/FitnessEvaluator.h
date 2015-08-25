#pragma once

#include <geneial/core/population/chromosome/BaseChromosome.h>

#include <memory>

//Forward Declaration for BaseChromosome (due to circular inclusion)
geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{
geneial_export_namespace
{
template<typename FITNESS_TYPE>
class BaseChromosome;
}
}
}
}

geneial_private_namespace(geneial)
{
using ::geneial::population::chromosome::BaseChromosome;

geneial_export_namespace
{

//TODO (bewo) Allow a lambda function as fitness evaluator?

template<typename FITNESS_TYPE>
class FitnessEvaluator : public std::enable_shared_from_this<FitnessEvaluator<FITNESS_TYPE>>
{
public:
    using ptr = std::shared_ptr<FitnessEvaluator<FITNESS_TYPE>>;

    using const_ptr = std::shared_ptr<const FitnessEvaluator<FITNESS_TYPE>>;

    ptr getPtr()
    {
        return this->shared_from_this();
    }

    const_ptr getConstPtr()
    {
        return this->shared_from_this();
    }


    FitnessEvaluator()
    {
    }

    virtual ~FitnessEvaluator()
    {
    }

    virtual std::unique_ptr<Fitness<FITNESS_TYPE>> evaluate(
            const BaseChromosome<FITNESS_TYPE>& chromosome) const = 0;
};

} /* geneial_export_namespace*/
} /* private namespace geneial */
