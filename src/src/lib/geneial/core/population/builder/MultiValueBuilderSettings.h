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

    unsigned int _num;

public:
    MultiValueBuilderSettings(const typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator, unsigned int num,
            VALUE_TYPE random_max, VALUE_TYPE random_min) :
            BuilderSettings<FITNESS_TYPE>(fitnessEvaluator), _random_max(random_max), _random_min(random_min), _num(num)
    {
    }

    virtual ~MultiValueBuilderSettings()
    {
    }

    inline unsigned int getNum() const
    {
        return _num;
    }

    void setNum(unsigned int num)
    {
        _num = num;
    }

    inline VALUE_TYPE getRandomMax() const
    {
        return _random_max;
    }

    void inline setRandomMax(VALUE_TYPE randomMax)
    {
        _random_max = randomMax;
    }

    inline VALUE_TYPE getRandomMin() const
    {
        return _random_min;
    }

    void setRandomMin(VALUE_TYPE randomMin)
    {
        _random_min = randomMin;
    }
};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */

