#pragma once

#include <geneial/core/population/chromosome/BaseChromosome.h>

#include <memory>

//Forward Declaration for BaseChromosome (due to circular inclusion)
namespace geneial
{
namespace population
{
namespace chromosome
{
template<typename FITNESS_TYPE>
class BaseChromosome;
}
}
}

namespace geneial
{

using namespace geneial::population::chromosome;

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

} /* namespace GeneticLibrary */

