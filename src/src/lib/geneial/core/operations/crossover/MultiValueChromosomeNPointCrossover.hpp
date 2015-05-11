#pragma once

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/utility/Random.h>

#include <set>
#include <algorithm>
#include <iterator>

namespace geneial
{
namespace operation
{
namespace crossover
{

using namespace geneial::operation::coupling;

//TODO (bewo): reduce cyclomatic complexity...
template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set
    MultiValueChromosomeNPointCrossover<VALUE_TYPE,FITNESS_TYPE>::doCrossover(
        const typename BaseChromosome<FITNESS_TYPE>::const_ptr &mommy,
        const typename BaseChromosome<FITNESS_TYPE>::const_ptr &daddy) const
{

    typedef typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::value_container value_container;
    typedef typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr mvc_ptr;
    typedef typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr mvc_cptr;

    typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set resultset;

    const mvc_cptr mvc_mommy = std::dynamic_pointer_cast<const MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(mommy);
    assert(mvc_mommy);

    const mvc_cptr mvc_daddy = std::dynamic_pointer_cast<const MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(daddy);
    assert(mvc_daddy);

    const unsigned int crossoverPoints = this->getCrossoverSettings().getCrossOverPoints();
    const unsigned int totalWidth = this->getBuilderSettings().getNum();
    std::set<unsigned int> crossoverPositions;
    if (this->getCrossoverSettings().getWidthSetting()
            == MultiValueChromosomeNPointCrossoverSettings::EQUIDISTANT_WIDTH)
    {

        const unsigned int equidistantwidth = totalWidth / (crossoverPoints + 1);
        for (unsigned int i = 0; i < crossoverPoints; i++)
        {
            crossoverPositions.insert(i * equidistantwidth);
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

        for (unsigned int i = 0; i < crossoverPoints; i++)
        {
            //Pick a random number in the interval
            unsigned int rnd_pos;
            bool valid;

            do
            {
                rnd_pos = Random::generate<int>(0, this->getBuilderSettings().getNum());

                std::set<unsigned int>::iterator itlow = std::lower_bound(crossoverPositions.begin(),
                        crossoverPositions.end(), rnd_pos);

                if (itlow != crossoverPositions.end())
                {
                    std::advance(itlow, -1);
                }

                std::set<unsigned int>::const_iterator itup = crossoverPositions.upper_bound(rnd_pos);
                std::set<unsigned int>::const_iterator pos = crossoverPositions.find(rnd_pos);

                valid = true;

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

                valid &= pos == crossoverPositions.end(); //element is not contained within the set
                if (!valid)
                {
                    continue;
                }

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

            } while (!valid);

            crossoverPositions.insert(rnd_pos);
        }
    }
    assert(crossoverPositions.size() == crossoverPoints);

    mvc_ptr child_candidate = std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(
            this->getBuilderFactory().createChromosome(BaseChromosomeFactory<FITNESS_TYPE>::LET_UNPOPULATED));
    assert(child_candidate);

    const value_container &daddy_container = mvc_daddy->getContainer();
    const value_container &mommy_container = mvc_mommy->getContainer();
    value_container &child_container = child_candidate->getContainer();

    child_container.clear();

    assert(daddy_container.size() == mommy_container.size());

    typename std::back_insert_iterator<value_container> target_it = std::back_inserter(child_container);
    std::set<unsigned int>::const_iterator widthIterator = crossoverPositions.begin();
    bool flip = true; //copy from ladies first.

    crossoverPositions.insert(daddy_container.size());

    unsigned int i = 0;

    for (; widthIterator != crossoverPositions.end(); ++widthIterator)
    {
        if (flip)
        {
            std::copy(mommy_container.begin() + i, mommy_container.begin() + *widthIterator, target_it);
        }
        else
        {
            std::copy(daddy_container.begin() + i, daddy_container.begin() + *widthIterator, target_it);
        }
        i = *widthIterator;
        flip = !flip;
    }
    assert(child_container.size() == mommy_container.size());

    resultset.push_back(child_candidate);

    return resultset;
}

} /* namespace crossover */
} /* namespace operation */
} /* namespace geneial */

