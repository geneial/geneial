#pragma once

#include <geneial/namespaces.h>

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

geneial_export_namespace
{
class MultiValueChromosomeNPointCrossoverSettings
{
public:

    /**
     * Every selection operation should allow the following modes:
     *     RANDOM_WIDTH -- Crossoverwidth are chosen at random
     *     RANDOM_MIN_WIDTH -- Crossoverwidth are chosen at random, but a min width has to be respected
     *     EQUIDISTANT_WIDTH -- Crossoverwith is equidistant
     *
     *
     * Example:
     * ========
     *
     * AAAAAAAAAAAAAAA
     *
     * BBBBBBBBBBBBBBB
     *
     * given a 2-Point-Crossover
     *
     * Equidistant width child
     *
     * width = 5
     * BBBBB|AAAAA|BBBBB
     *
     * RANDOM_MIN_WIDTH, min. width = 2
     * widths = {5,2,8}
     *
     * BBBBB|AA|BBBBBBBB
     *
     * RANDOM_WIDTH,
     * widths = {2,2,13}
     *
     * BB|AA|BBBBBBBBBBB
     *
     */
    typedef enum
    {
        RANDOM_WIDTH, RANDOM_MIN_WIDTH, EQUIDISTANT_WIDTH
    } width_settings;

    MultiValueChromosomeNPointCrossoverSettings(const unsigned int crossOverPoints, const width_settings width_setting,
            const unsigned int min_width = 1) :
            _crossOverPoints(crossOverPoints), _width_setting(width_setting), _min_width(min_width)
    {
        assert(crossOverPoints > 0);
        assert(width_setting != RANDOM_MIN_WIDTH || min_width > 1);
    }

    virtual ~MultiValueChromosomeNPointCrossoverSettings()
    {
    }

    unsigned int getCrossOverPoints() const
    {
        return _crossOverPoints;
    }

    void setCrossOverPoints(const unsigned int crossOverPoints)
    {
        _crossOverPoints = crossOverPoints;
    }

    unsigned int getMinWidth() const
    {
        return _min_width;
    }

    void setMinWidth(const unsigned int minWidth)
    {
        _min_width = minWidth;
    }

    width_settings getWidthSetting() const
    {
        return _width_setting;
    }

    void setWidthSetting(const width_settings widthSetting)
    {
        _width_setting = widthSetting;
    }

private:
    unsigned int _crossOverPoints;
    width_settings _width_setting;
    unsigned int _min_width;

};

} /* geneial_export_namespace */
} /* private namespace crossover */
} /* private namespace operation */
} /* private namespace geneial */
