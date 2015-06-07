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
    ContinousMultiValueBuilderSettings(const typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator,
                                       const unsigned int num,
                                       const VALUE_TYPE random_max,
                                       const  VALUE_TYPE random_min,
                                       const bool hasStart = false,
                                       const VALUE_TYPE start = 0,
                                       const VALUE_TYPE eps = 1) :
            MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>(fitnessEvaluator, num, random_max, random_min),
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

    bool isHasStart() const
    {
        return _hasStart;
    }

    void setHasStart(bool hasStart)
    {
        _hasStart = hasStart;
    }

    VALUE_TYPE hasStart() const
    {
        return _start;
    }

    VALUE_TYPE getStart() const
    {
        return _start;
    }

    void setStart(VALUE_TYPE start)
    {
        _start = start;
    }
};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */
