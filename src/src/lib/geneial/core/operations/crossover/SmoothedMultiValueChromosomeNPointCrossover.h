#pragma once

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/core/population/builder/ContinousMultiIntValueChromosomeFactory.h>
#include <geneial/utility/Smoothing.h>
#include <cassert>

namespace geneial
{
namespace operation
{
namespace crossover
{

using namespace geneial::utility;
using namespace geneial::population::chromosome;

//TODO (bewo) allow random crossover width per settings

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class SmoothedMultiValueChromosomeNPointCrossover: public MultiValueChromosomeNPointCrossover<VALUE_TYPE, FITNESS_TYPE>
{
private:
    ContinousMultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> *_builderSettings;
public:
    SmoothedMultiValueChromosomeNPointCrossover(MultiValueChromosomeNPointCrossoverSettings *crossoverSettings,
            ContinousMultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> *builderSettings,
            ContinousMultiIntValueChromosomeFactory<FITNESS_TYPE> *builderFactory //TODO (bewo) Clean this up!
            ) :
            MultiValueChromosomeNPointCrossover<VALUE_TYPE, FITNESS_TYPE>(crossoverSettings, builderSettings,
                    builderFactory), _builderSettings(builderSettings)
    {
    }

    virtual ~SmoothedMultiValueChromosomeNPointCrossover()
    {
    }

    virtual bool inline isSymmetric() const
    {
        return false;
    }

    virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set doCrossover(
            typename BaseChromosome<FITNESS_TYPE>::ptr mommy, typename BaseChromosome<FITNESS_TYPE>::ptr daddy)
    {
        typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set result =
                MultiValueChromosomeNPointCrossover<VALUE_TYPE, FITNESS_TYPE>::doCrossover(mommy, daddy);

        for (typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set::iterator it = result.begin();
                it != result.end(); ++it)
        {
            Smoothing::restoreSmoothness<VALUE_TYPE, FITNESS_TYPE>(
                    boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(*it),
                    _builderSettings->getEps(), _builderSettings->getRandomMin(), _builderSettings->getRandomMax());

        }

        return result;

    }

};

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.hpp>

