#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/coupling/BaseCouplingOperation.h>
#include <geneial/utility/mixins/EnableMakeShared.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(coupling)
{
using ::geneial::population::Population;
using ::geneial::operation::crossover::BaseCrossoverOperation;
using ::geneial::operation::selection::BaseSelectionOperation;
using ::geneial::population::management::BaseManager;
using ::geneial::utility::EnableMakeShared;

geneial_export_namespace
{

/**
 * @brief This class encapsulates a lambda function doing the crossover
 */
template<typename FITNESS_TYPE>
class LambdaCouplingAdapter:    public BaseCouplingOperation<FITNESS_TYPE>,
                                public virtual EnableMakeShared<LambdaCouplingAdapter<FITNESS_TYPE>>
{
public:
    using offspring_result_set = typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set;
    using selection_result_set = typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set;
    using function_type = typename std::function<offspring_result_set(
            const selection_result_set&,
            const BaseCrossoverOperation<FITNESS_TYPE> &crossoverOperation,
            BaseManager<FITNESS_TYPE> &manager
    )>;

    virtual ~LambdaCouplingAdapter()
    {
    }

    virtual offspring_result_set doCopulate(const selection_result_set &mating_pool,
            const BaseCrossoverOperation<FITNESS_TYPE> &crossoverOperation, BaseManager<FITNESS_TYPE> &manager) override
    {
        return _lmbda(mating_pool, crossoverOperation, manager);
    }

    class Builder: public BaseCouplingOperation<FITNESS_TYPE>::Builder
    {
        function_type _function;
    public:

        Builder(function_type func) :
                BaseCouplingOperation<FITNESS_TYPE>::Builder(), _function(func)
        {
        }

        virtual typename BaseCouplingOperation<FITNESS_TYPE>::ptr create() override
        {
            return LambdaCouplingAdapter<FITNESS_TYPE>::makeShared(_function);
        }

        const function_type& getFunction() const
        {
            return _function;
        }

        Builder& setFunction(const function_type& function)
        {
            _function = function;
            return *this;
        }
    };
private:
    function_type _lmbda;
protected:
    explicit LambdaCouplingAdapter(const function_type lmbda) :
            BaseCouplingOperation<FITNESS_TYPE>(std::make_shared<CouplingSettings>()), _lmbda(lmbda)
    {
    }
};

} /* geneial_export_namespace */
} /* private namespace coupling */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/coupling/RandomCouplingOperation.hpp>

