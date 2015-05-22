#pragma once

#include <geneial/core/operations/crossover/MultiValueChromosomeCrossoverOperation.h>
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
class MultiValueChromosomeNPointCrossover: public MultiValueChromosomeCrossoverOperation<VALUE_TYPE,FITNESS_TYPE>
{
private:
    std::shared_ptr<const MultiValueChromosomeNPointCrossoverSettings> _crossoverSettings;

public:
    MultiValueChromosomeNPointCrossover(
            const std::shared_ptr<const MultiValueChromosomeNPointCrossoverSettings> &crossoverSettings,
            const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory):
                MultiValueChromosomeCrossoverOperation<VALUE_TYPE,FITNESS_TYPE>(builderFactory),
            _crossoverSettings(crossoverSettings)
    {
        //ensure the crossoverpoints does not exceed the number of values.
        assert(getCrossoverSettings().getCrossOverPoints() <= this->getBuilderSettings().getNum());

        //ensure when min width is selected that the min width does not exceed the amount of spaces we have.
        assert(
                getCrossoverSettings().getWidthSetting()
                        != MultiValueChromosomeNPointCrossoverSettings::RANDOM_MIN_WIDTH
                        || getCrossoverSettings().getMinWidth() * getCrossoverSettings().getCrossOverPoints()
                                <= this->getBuilderSettings().getNum());

    }

    virtual ~MultiValueChromosomeNPointCrossover()
    {
    }

    virtual bool inline isSymmetric() const override
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
    virtual typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set doMultiValueCrossover(
            const typename MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
            const typename MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const override;

    inline const MultiValueChromosomeNPointCrossoverSettings & getCrossoverSettings() const
    {
        return *_crossoverSettings;
    }

    void setCrossoverSettings(const MultiValueChromosomeNPointCrossoverSettings & crossoverSettings)
    {
        _crossoverSettings = crossoverSettings;
    }

};

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.hpp>

