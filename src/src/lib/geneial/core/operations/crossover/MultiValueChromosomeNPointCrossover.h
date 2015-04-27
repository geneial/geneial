#pragma once

#include <geneial/core/operations/crossover/BaseCrossoverOperation.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossoverSettings.h>

#include <cassert>

namespace geneial
{
namespace operation
{
namespace crossover
{

//TODO (bewo) allow random crossover width per settings

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeNPointCrossover: public BaseCrossoverOperation<FITNESS_TYPE>
{
private:
    MultiValueChromosomeNPointCrossoverSettings * _crossoverSettings;
    MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> * _builderSettings;
    MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> *_builderFactory;

public:
    MultiValueChromosomeNPointCrossover(MultiValueChromosomeNPointCrossoverSettings *crossoverSettings,
            MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> *builderSettings,
            MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> *builderFactory) :
            _crossoverSettings(crossoverSettings), _builderSettings(builderSettings), _builderFactory(builderFactory)
    {
        assert(_crossoverSettings != NULL);
        assert(_builderSettings != NULL);
        assert(_builderFactory != NULL);

        //ensure the crossoverpoints does not exceed the number of values.
        assert(_crossoverSettings->getCrossOverPoints() <= this->getBuilderSettings()->getNum());

        //ensure when minwidth is selected that the minwidth does not exceed the amount of spaces we have.
        assert(
                _crossoverSettings->getWidthSetting() != MultiValueChromosomeNPointCrossoverSettings::RANDOM_MIN_WIDTH
                        || _crossoverSettings->getMinWidth() * _crossoverSettings->getCrossOverPoints()
                                <= this->getBuilderSettings()->getNum());

    }
    virtual ~MultiValueChromosomeNPointCrossover()
    {
    }

    virtual bool inline isSymmetric() const
    {
        return false;
    }

    /****
     * Computes the N-Point crossover of two multi value chromosomes.
     *
     * Example:
     * Say, A and B are Chromosomes with
     *
     * A (mommy)
     * ===========================
     * |A1|A2|A3|A4|A5|A6|A7|A8|A9
     * ===========================
     *
     * B (daddy)
     * ===========================
     * |B1|B2|B3|B4|B5|B6|B7|B8|B9
     * ===========================
     *
     * With a 2-Point-Crossover the resulting chromosome is
     *
     * Child    |        |
     * ===========================
     * |A1|A2|A3|B4|B5|B6|A7|A8|A9
     * ===========================
     *
     */
    virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set
    doCrossover(typename BaseChromosome<FITNESS_TYPE>::ptr mommy, typename BaseChromosome<FITNESS_TYPE>::ptr daddy);

    MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>* const & getBuilderSettings() const
    {
        return _builderSettings;
    }

    void setBuilderSettings(const MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>* & builderSettings)
    {
        _builderSettings = builderSettings;
    }

    MultiValueChromosomeNPointCrossoverSettings* const & getCrossoverSettings() const
    {
        return _crossoverSettings;
    }

    void setCrossoverSettings(const MultiValueChromosomeNPointCrossoverSettings* & crossoverSettings)
    {
        _crossoverSettings = crossoverSettings;
    }

    MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>* const & getBuilderFactory() const
    {
        return _builderFactory;
    }

    void setBuilderFactory(const MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>* & builderFactory)
    {
        _builderFactory = builderFactory;
    }

};

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.hpp>

