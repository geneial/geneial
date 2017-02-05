#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/builder/MultiValueBuilderSettings.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{

geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class PermutationBuilderSettings: public MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>
{
private:
	unsigned int _permutationRoundsMin;
	unsigned int _permutationRoundsMax;

public:
    PermutationBuilderSettings(const typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator,
							   const unsigned int num = MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>::DEFAULT_NUM,
							   const unsigned int permutationRoundsMin = MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>::DEFAULT_NUM,
							   const unsigned int permutationdsRoundsMax = 2 * MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>::DEFAULT_NUM ):
    		MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>(fitnessEvaluator, num, -1, -1), //TODO (bewo): Fix this -1,-1 hack
			_permutationRoundsMin(permutationRoundsMin),
			_permutationRoundsMax(permutationdsRoundsMax)
    {
    }

    inline unsigned int getPermutationRoundsMax() const
    {
        return _permutationRoundsMax;
    }

    inline void setPermutationRoundsMax(unsigned int permutationRoundsMax)
    {
        _permutationRoundsMax = permutationRoundsMax;
    }

    inline unsigned int getPermutationRoundsMin() const
    {
        return _permutationRoundsMin;
    }

    inline void setPermutationRoundsMin(unsigned int permutationRoundsMin)
    {
        _permutationRoundsMax = permutationRoundsMin;
    }


    virtual ~PermutationBuilderSettings()
    {
    }

};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */
