#pragma once

#include <geneial/core/operations/crossover/MultiValueChromosomeBlendingCrossover.h>
#include <geneial/core/operations/crossover/BaseCrossoverOperation.h>
#include <geneial/utility/Random.h>

namespace geneial
{
namespace operation
{
namespace crossover
{

using namespace geneial::utility;
using namespace geneial::operation::coupling;

template<typename VALUE_TYPE, typename FITNESS_TYPE>
inline typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set MultiValueChromosomeBlendingCrossover<
        VALUE_TYPE, FITNESS_TYPE>::doCrossover(typename BaseChromosome<FITNESS_TYPE>::ptr mommy,
        typename BaseChromosome<FITNESS_TYPE>::ptr daddy)
{
    //Cast Chromosomes, shorthands for necessary data structures...
    typedef typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::value_container value_container;
    typedef typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr mvc_ptr;

    typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set resultset;

    const mvc_ptr mvc_mommy = boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(mommy);
    assert(mvc_mommy);

    const mvc_ptr mvc_daddy = boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(daddy);
    assert(mvc_daddy);

    //Determine the amount of chromosomes to generate
    unsigned int chromosomesToGenerate;

    if (_offspringMode == FIXED_AMOUNT)
    {
        chromosomesToGenerate = _numChilds;
    }
    else
    {
        chromosomesToGenerate = Random::instance()->generateInt(1, _numChilds);
    }

    for (unsigned int i = 0; i < chromosomesToGenerate; i++)
    {
        InterpolateBeta interpolationMethod = _interpolationMethod;

        if (_interpolationMethod == INTERPOLATE_RANDOM)
        {
            interpolationMethod = (InterpolateBeta) (INTERPOLATE_RANDOM + Random::instance()->generateInt(1, 3));
        }

        const double beta1 = Random::instance()->generateDouble(0.0, 1.0);
        const double beta2 = Random::instance()->generateDouble(0.0, 1.0);

        mvc_ptr child_candidate = boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(
                this->getBuilderFactory()->createChromosome(BaseChromosomeFactory<FITNESS_TYPE>::LET_UNPOPULATED));

        assert(child_candidate);

        const unsigned int containerSize = this->getBuilderSettings()->getNum();

        value_container &daddy_container = mvc_daddy->getContainer();
        value_container &mommy_container = mvc_mommy->getContainer();
        value_container &child_container = child_candidate->getContainer();

        typename value_container::iterator mommy_it = mommy_container.begin();
        typename value_container::iterator daddy_it = daddy_container.begin();
        for (unsigned int i = 0; mommy_it != mommy_container.end(); ++i)
        {
            double targetBeta;
            switch (interpolationMethod)
            {
            case INTERPOLATE_COSINE:
            {
                targetBeta = Interpolators::cosineInterpolate<double>(beta1, beta2,
                        ((double) i) / ((double) containerSize));
                break;
            }
            case INTERPOLATE_LINEARLY:
            {
                targetBeta = Interpolators::linearInterpolate<double>(beta1, beta2,
                        ((double) i) / ((double) containerSize));
                break;
            }
            default: //FALLTRHOUGH INTENDED
            case INTERPOLATE_NONE:
            {
                targetBeta = beta1;
                break;
            }
            }
            assert(targetBeta <= 1.0);
            assert(targetBeta >= 0);
            child_container.push_back((*mommy_it * (targetBeta) + *daddy_it * (targetBeta)));
            ++mommy_it;
            ++daddy_it;
        }

        resultset.push_back(child_candidate);
    }

    return resultset;
}

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */

