#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/core/population/Population.h>

#include <memory>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(crossover)
{
using ::geneial::population::Population;

geneial_export_namespace
{
template<typename FITNESS_TYPE>
class BaseCrossoverOperation
{

protected:

    BaseCrossoverOperation()
    {
    }

public:

    using crossover_result_set = typename Population<FITNESS_TYPE>::chromosome_container ;

    using ptr = std::shared_ptr<BaseCrossoverOperation<FITNESS_TYPE>>;

    using const_ptr = std::shared_ptr<BaseCrossoverOperation<FITNESS_TYPE>>;


    virtual ~BaseCrossoverOperation()
    {
    }

    virtual crossover_result_set
    doCrossover(const typename BaseChromosome<FITNESS_TYPE>::const_ptr &mommy, const typename BaseChromosome<FITNESS_TYPE>::const_ptr &daddy) const = 0;

    //Is the order of mommy for and daddy for doCrossover() relevant?, true if yes.
    //TODO(bewo): Think about encoding this property via inheritance?
    virtual bool inline isSymmetric() const = 0;

    class Builder
    {
    public:
        virtual ptr create() = 0;
    };

};

} /* geneial_export_namespace */
} /* private namespace crossover */
} /* private namespace operation */
} /* private namespace geneial */

