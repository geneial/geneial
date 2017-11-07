#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/builder/BuilderSettings.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{

geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueBuilderSettings: public BuilderSettings<FITNESS_TYPE>
{
private:
    VALUE_TYPE _random_max;

    VALUE_TYPE _random_min;

    unsigned int _numberallee;

public:

    // static constexpr const VALUE_TYPE DEFAULT_RANDOM_MAX = 100;
		//
    // static constexpr const VALUE_TYPE DEFAULT_RANDOM_MIN = 0;

    static constexpr const unsigned int DEFAULT_NUM = 2;

    MultiValueBuilderSettings( const typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator,
                               const unsigned int num,
                               const VALUE_TYPE random_max,
                               const VALUE_TYPE random_min) :
            BuilderSettings<FITNESS_TYPE>(fitnessEvaluator),
            _random_max(random_max),
            _random_min(random_min),
            _numberallee(num)
    {
    }

		MultiValueBuilderSettings( const typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator) :
						BuilderSettings<FITNESS_TYPE>(fitnessEvaluator)
		{
		}


    virtual ~MultiValueBuilderSettings()
    {
    }

    inline unsigned int getNum() const
    {
        return _numberallee;
    }

    inline void setNum(unsigned int num)
    {
        _numberallee = num;
    }

    inline VALUE_TYPE getRandomMax() const
    {
        return _random_max;
    }

    inline void setRandomMax(VALUE_TYPE randomMax)
    {
        _random_max = randomMax;
    }

    inline VALUE_TYPE getRandomMin() const
    {
        return _random_min;
    }

    inline void setRandomMin(VALUE_TYPE randomMin)
    {
        _random_min = randomMin;
    }
};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */
