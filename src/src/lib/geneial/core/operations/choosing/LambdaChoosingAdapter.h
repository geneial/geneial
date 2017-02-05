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
class LambdaChoosingAdapter:    public BaseChoosingOperation<FITNESS_TYPE>,
                                public EnableMakeShared<LambdaChoosingAdapter<FITNESS_TYPE>>
{
public:
    using input_set = typename Population<FITNESS_TYPE>::chromosome_container;
    using output_set = typename Population<FITNESS_TYPE>::chromosome_container;
    using function_type = typename std::function<output_set(const input_set&)>;

    virtual ~LambdaChoosingAdapter()
    {
    }

    virtual output_set doChoose(const input_set&chromosomeInputSet) const override
    {
        return _function(chromosomeInputSet);
    }

    class Builder: public BaseChoosingOperation<FITNESS_TYPE>::Builder
    {
    private:
        function_type _function;
    public:
        const constexpr static double DEFAULT_PROBABILITY = 0.1;

        Builder(function_type func) :
            BaseChoosingOperation<FITNESS_TYPE>::Builder(), _function(func)
        {
        }

        virtual typename BaseChoosingOperation<FITNESS_TYPE>::ptr create() override
        {
            return LambdaChoosingAdapter<FITNESS_TYPE>::makeShared(_function);
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
	function_type _function;
protected:
    explicit LambdaChoosingAdapter(const function_type& function) :
            BaseChoosingOperation<FITNESS_TYPE>(),
            _function(function)
    {
    }
};

} /* geneial_export_namespace */
} /* private namespace choosing */
} /* private namespace operation */
} /* private namespace geneial */


#include <geneial/core/operations/choosing/ChooseRandom.hpp>

