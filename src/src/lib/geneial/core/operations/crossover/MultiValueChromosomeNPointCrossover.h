#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeCrossoverOperation.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossoverSettings.h>
#include <geneial/utility/mixins/EnableMakeShared.h>

#include <cassert>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(crossover)
{
using ::geneial::population::Population;
using ::geneial::population::chromosome::MultiValueChromosome;
using ::geneial::operation::coupling::BaseCouplingOperation;
using ::geneial::utility::EnableMakeShared;


geneial_export_namespace
{
//TODO (bewo) allow random crossover width per settings

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeNPointCrossover:  public MultiValueChromosomeCrossoverOperation<VALUE_TYPE,FITNESS_TYPE>,
                                            public virtual EnableMakeShared<MultiValueChromosomeNPointCrossover<VALUE_TYPE,FITNESS_TYPE>>
{
private:
    std::shared_ptr<const MultiValueChromosomeNPointCrossoverSettings> _crossoverSettings;

protected:
    MultiValueChromosomeNPointCrossover(
            const std::shared_ptr<const MultiValueChromosomeNPointCrossoverSettings> &crossoverSettings,
            const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory):
                MultiValueChromosomeCrossoverOperation<VALUE_TYPE,FITNESS_TYPE>(builderFactory),
            _crossoverSettings(crossoverSettings)
    {
        //ensure the crossoverpoints does not exceed the number of values.
        assert(getCrossoverSettings().getCrossOverPoints() <= this->getBuilderFactory().getSettings().getNum());

        //ensure when min width is selected that the min width does not exceed the amount of spaces we have.
        assert(
                getCrossoverSettings().getWidthSetting()
                        != MultiValueChromosomeNPointCrossoverSettings::RANDOM_MIN_WIDTH
                        || getCrossoverSettings().getMinWidth() * getCrossoverSettings().getCrossOverPoints()
                                <= this->getBuilderFactory().getSettings().getNum());

    }

public:

    virtual ~MultiValueChromosomeNPointCrossover()
    {
    }

    virtual bool isSymmetric() const override
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
            const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
            const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const override;

    inline const MultiValueChromosomeNPointCrossoverSettings & getCrossoverSettings() const
    {
        return *_crossoverSettings;
    }

    void setCrossoverSettings(std::shared_ptr<const MultiValueChromosomeNPointCrossoverSettings>& crossoverSettings)
    {
        _crossoverSettings = crossoverSettings;
    }



    class Builder : public MultiValueChromosomeCrossoverOperation<VALUE_TYPE,FITNESS_TYPE>::Builder
    {
    protected:
        std::shared_ptr<MultiValueChromosomeNPointCrossoverSettings> _crossoverSettings;

    public:
        Builder(const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> & builderFactory) :
                MultiValueChromosomeCrossoverOperation<VALUE_TYPE, FITNESS_TYPE>::Builder(builderFactory), _crossoverSettings(
                        new MultiValueChromosomeNPointCrossoverSettings(DEFAULT_CROSSOVERPOINTS, DEFAULT_WIDTH_MODE))
        {
        }

        const static unsigned int DEFAULT_CROSSOVERPOINTS = 1;

        const static MultiValueChromosomeNPointCrossoverSettings::width_settings DEFAULT_WIDTH_MODE = MultiValueChromosomeNPointCrossoverSettings::RANDOM_WIDTH;

        inline MultiValueChromosomeNPointCrossoverSettings & getCrossoverSettings()
            {
                return *_crossoverSettings;
            }

        virtual typename BaseCrossoverOperation<FITNESS_TYPE>::ptr create() override
        {
            if(! this->_builderFactory )
            {
                throw new std::runtime_error("Must set a Chromosome Factory to build MultiValueCrossover");
            }

            return MultiValueChromosomeNPointCrossover::makeShared(
                                        _crossoverSettings,
                                        this->_builderFactory);
        }
    };
};

} /* geneial_export_namespace */
} /* private namespace crossover */
} /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.hpp>

