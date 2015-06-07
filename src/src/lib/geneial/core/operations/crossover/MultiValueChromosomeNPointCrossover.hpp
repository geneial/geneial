#pragma once

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/utility/Random.h>

#include <set>
#include <algorithm>
#include <iterator>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(crossover)
{
using ::geneial::population::Population;
using ::geneial::population::chromosome::MultiValueChromosome;
using ::geneial::operation::coupling::BaseCouplingOperation;
using ::geneial::utility::Random;

geneial_export_namespace
{

//TODO (bewo): reduce cyclomatic complexity...
template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set MultiValueChromosomeNPointCrossover<VALUE_TYPE,
        FITNESS_TYPE>::doMultiValueCrossover(
        const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
        const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const
{

    typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set resultset;


    const unsigned int crossoverPoints = this->getCrossoverSettings().getCrossOverPoints();
    const unsigned int totalWidth = this->getBuilderFactory().getSettings().getNum();

    //std::set<unsigned int> crossoverPositions;

    std::vector<unsigned int> crossoverPositions;
    crossoverPositions.reserve(crossoverPoints);



    if (this->getCrossoverSettings().getWidthSetting()
            == MultiValueChromosomeNPointCrossoverSettings::EQUIDISTANT_WIDTH)
    {
        const unsigned int equidistantwidth = totalWidth / (crossoverPoints + 1);

        for (unsigned int i = 0; i < crossoverPoints; i++)
        {
            crossoverPositions.emplace_back(i * equidistantwidth);
        }

    }
    else if (this->getCrossoverSettings().getWidthSetting()
            == MultiValueChromosomeNPointCrossoverSettings::RANDOM_WIDTH
            || this->getCrossoverSettings().getWidthSetting()
                    == MultiValueChromosomeNPointCrossoverSettings::RANDOM_MIN_WIDTH)
    {

        unsigned int minWidth;
        if (this->getCrossoverSettings().getWidthSetting()
                == MultiValueChromosomeNPointCrossoverSettings::RANDOM_WIDTH)
        {
            minWidth = 1;
        }
        else
        {
            minWidth = this->getCrossoverSettings().getMinWidth();
        }

        //TODO(bewo) make some assertions about minwidth, crossoverPositions and size

        const auto max = this->getBuilderFactory().getSettings().getNum();
        while (crossoverPositions.size() < crossoverPoints)
        {
            const unsigned int rnd_pos = Random::generate<int>(0, max);
            bool valid = true;

            valid &= rnd_pos >= minWidth; //ensure there is enough space on the interval boundaries
            if (!valid)
            {
                continue;
            }

            valid &= rnd_pos <= totalWidth - minWidth;
            if (!valid)
            {
                continue;
            }

            const auto pos = std::find(crossoverPositions.begin(), crossoverPositions.end(), rnd_pos);
            valid &= pos == crossoverPositions.end(); //element is not contained within the set
            if (!valid)
            {
                continue;
            }

            const auto itlow = std::lower_bound(crossoverPositions.begin(), crossoverPositions.end(), rnd_pos);
            const auto itup = std::upper_bound(crossoverPositions.begin(), crossoverPositions.end(), rnd_pos);
            //is the picked element too near to another element already contained?
            //ensure between two values there is enough width, i.e. either we have a lower or upper neighbor and the distance is correct or not.
            valid &= itlow == crossoverPositions.end()
                    || (itlow != crossoverPositions.end() && rnd_pos - *itlow >= minWidth);
            if (!valid)
            {
                continue;
            }

            valid &= itup == crossoverPositions.end()
                    || (itup != crossoverPositions.end() && *itup - rnd_pos >= minWidth);
            if (!valid)
            {
                continue;
            }
            crossoverPositions.emplace_back(rnd_pos);
        }

    std::sort(crossoverPositions.begin(),crossoverPositions.end());
    }


    assert(crossoverPositions.size() == crossoverPoints);

    auto child_candidate = this->createChildCandidate();

    const auto &daddy_container = daddy->getContainer();
    const auto &mommy_container = mommy->getContainer();
    auto &child_container = child_candidate->getContainer();

    assert(daddy_container.size() == mommy_container.size());

    bool flip = true; //copy from ladies first.

    //push back to end so its ok, does not destroy sortedness
    crossoverPositions.push_back(daddy_container.size());

    auto widthIterator = crossoverPositions.cbegin();

    unsigned int i = 0;

    for (; widthIterator != crossoverPositions.end(); ++widthIterator)
    {
        if (flip)
        {
            std::copy(mommy_container.begin() + i, mommy_container.begin() + *widthIterator, child_container.begin()+i);
        }
        else
        {
            std::copy(daddy_container.begin() + i, daddy_container.begin() + *widthIterator, child_container.begin()+i);
        }
        i = *widthIterator;
        flip = !flip;
    }
    assert(child_container.size() == mommy_container.size());

    resultset.emplace_back(std::move(child_candidate));

    return std::move(resultset);
}

} /* geneial_export_namespace */
} /* private namespace crossover */
} /* private namespace operation */
} /* private namespace geneial */
