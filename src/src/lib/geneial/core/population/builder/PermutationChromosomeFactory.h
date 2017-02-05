#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/core/population/builder/PermutationBuilderSettings.h>
#include <geneial/utility/mixins/EnableMakeShared.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{
using ::geneial::utility::EnableMakeShared;

geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class PermutationChromosomeFactory: public MultiValueChromosomeFactory<VALUE_TYPE,FITNESS_TYPE>,
                                    public virtual EnableMakeShared<PermutationChromosomeFactory<VALUE_TYPE,FITNESS_TYPE>>
{
private:
    const PermutationBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &_settings;

public:

    class Builder : public BaseChromosomeFactory<FITNESS_TYPE>::Builder //NOTE (bewo): Do not inherit from MVC builder. (keep it simple)
        {

    	PermutationBuilderSettings<VALUE_TYPE, FITNESS_TYPE> _settings;

        public:

    		PermutationBuilderSettings<VALUE_TYPE,FITNESS_TYPE>& getSettings()
            {
                return _settings;
            }

            Builder(const typename FitnessEvaluator<FITNESS_TYPE>::ptr &fitnessEvaluator): _settings(fitnessEvaluator)
            {
            }

            virtual typename BaseChromosomeFactory<FITNESS_TYPE>::ptr create() override
            {
                return std::move(EnableMakeShared<PermutationChromosomeFactory<VALUE_TYPE,FITNESS_TYPE>>::makeShared(_settings));
            }

        };


protected:
    explicit PermutationChromosomeFactory(const PermutationBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &settings) :
        MultiValueChromosomeFactory<VALUE_TYPE,FITNESS_TYPE>(settings), _settings(settings)
    {
    }

    typename BaseChromosome<FITNESS_TYPE>::ptr doCreateChromosome(
            typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues
        ) override;
};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */

#include <geneial/core/population/builder/PermutationChromosomeFactory.hpp>

