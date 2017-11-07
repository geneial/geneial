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
class ContinousMultiValueBuilderSettings: public MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>
{
private:

    bool _hasStart; //Do we have a starting point?

    VALUE_TYPE _start; //the starting point from which to start the chromosome array

    VALUE_TYPE _eps; //eps delta deriation between values in the array

public:

    const static bool DEFAULT_HAS_START = false;
    const static bool DEFAULT_START = 0;
    const static bool DEFAULT_EPS = 1;

    ContinousMultiValueBuilderSettings(const typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator,
                                       const unsigned int num,
                                       const VALUE_TYPE random_max,
                                       const VALUE_TYPE random_min,
                                       const bool hasStart,
                                       const VALUE_TYPE start,
                                       const VALUE_TYPE eps) :
            MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>(fitnessEvaluator, num, random_max, random_min),
            _hasStart(hasStart),
            _start(start), //Start point for generating ContinousChromomsomes
            _eps(eps) //Eps derivation from previous point per step
    {
    }

		ContinousMultiValueBuilderSettings(const typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator,
                                       const bool hasStart = DEFAULT_HAS_START,
                                       const VALUE_TYPE start = DEFAULT_START,
                                       const VALUE_TYPE eps = DEFAULT_EPS) :
            MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>(fitnessEvaluator),
            _hasStart(hasStart),
            _start(start), //Start point for generating ContinousChromomsomes
            _eps(eps) //Eps derivation from previous point per step
    {
    }


    virtual ~ContinousMultiValueBuilderSettings()
    {
    }

    VALUE_TYPE getEps() const
    {
        return _eps;
    }

    void setEps(VALUE_TYPE eps)
    {
        _eps = eps;
    }

    void setHasStart(bool hasStart)
    {
        _hasStart = hasStart;
    }

    VALUE_TYPE hasStart() const
    {
        return _hasStart;
    }

    VALUE_TYPE getStartValue() const
    {
        return _start;
    }

    void setStartValue(VALUE_TYPE start)
    {
        _start = start;
    }
};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */
