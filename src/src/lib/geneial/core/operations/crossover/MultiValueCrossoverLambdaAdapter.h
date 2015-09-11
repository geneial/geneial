#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeCrossoverOperation.h>
#include <geneial/utility/mixins/EnableMakeShared.h>

#include <cassert>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(crossover)
{
using ::geneial::population::Population;
using ::geneial::population::chromosome::MultiValueChromosome;
using ::geneial::utility::EnableMakeShared;

geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueCrossoverLambdaAdapter: public MultiValueChromosomeCrossoverOperation<VALUE_TYPE,FITNESS_TYPE>,
                                        public virtual EnableMakeShared<MultiValueCrossoverLambdaAdapter<VALUE_TYPE,FITNESS_TYPE>>
{
private:
    const bool _operationIsSymmetric;
    const function_type _function;

protected:
    MultiValueCrossoverLambdaAdapter(
            const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory,
            const function_type &function,
            const bool isSymmetric = false
            ) : MultiValueChromosomeCrossoverOperation<VALUE_TYPE,FITNESS_TYPE>(builderFactory),
                    _function(function),
                    _operationIsSymmetric(isSymmetric)
    {
    }

public:
    using return_type = typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set;
    using arg_type = const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr&;

    using function_type =
            typename std::function
                                <return_type(
                                        arg_type mommy,
                                        arg_type daddy,
                                        MultiValueCrossoverLambdaAdapter<VALUE_TYPE,FITNESS_TYPE>&
                                        )>;

    virtual ~MultiValueCrossoverLambdaAdapter()
    {
    }

    bool isSymmetric() const override
    {
        return false;
    }

    virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set doMultiValueCrossover(
            const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
            const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const override
    {
        return _function(mommy,daddy,*this);
    }


    class Builder : public MultiValueChromosomeCrossoverOperation<VALUE_TYPE,FITNESS_TYPE>::Builder
    {
        bool _operationIsSymmetric;
        function_type _function;
    public:
        Builder(const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> & builderFactory,
                const function_type function,
                const bool operationIsSymmetric = false) : MultiValueChromosomeCrossoverOperation<VALUE_TYPE, FITNESS_TYPE>::Builder(builderFactory),
                 _function(function),
                 _operationIsSymmetric(operationIsSymmetric)
        {
        }

        virtual typename BaseCrossoverOperation<FITNESS_TYPE>::ptr create() override
        {
            if(! this->_builderFactory )
            {
                throw new std::runtime_error("Must set a Chromosome Factory to build MultiValueCrossover");
            }

            return MultiValueCrossoverLambdaAdapter<VALUE_TYPE, FITNESS_TYPE>::makeShared(this->_builderFactory,_function,_operationIsSymmetric);
        }
    };
};

} /* geneial_export_namespace */
} /* private namespace crossover */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/crossover/MultiValueChromosomeAverageCrossover.hpp>

