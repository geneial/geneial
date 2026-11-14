#pragma once
#include <algorithm>
#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/utility/Random.h>
#include <numeric>
#include <stdexcept>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(operation)
    {
        geneial_private_namespace(crossover)
        {
            geneial_export_namespace
            {

                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set
                MultiValueChromosomeNPointCrossover<VALUE_TYPE, FITNESS_TYPE>::doMultiValueCrossover(
                    const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
                    const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const
                {
                    const auto &maternal = mommy->getContainer();
                    const auto &paternal = daddy->getContainer();
                    const auto width = maternal.size();
                    const std::size_t points = this->getCrossoverSettings().getCrossOverPoints();
                    const auto mode = this->getCrossoverSettings().getWidthSetting();
                    const std::size_t minimum = mode == MultiValueChromosomeNPointCrossoverSettings::RANDOM_MIN_WIDTH
                                                    ? this->getCrossoverSettings().getMinWidth()
                                                    : 1;
                    if (width != paternal.size() || width != this->getBuilderFactory().getSettings().getNum() ||
                        minimum == 0 || points == 0 || points >= width || minimum > width / (points + 1))
                    {
                        throw std::invalid_argument("Crossover points and minimum widths must fit both parents");
                    }
                    std::vector<std::size_t> positions;
                    if (mode == MultiValueChromosomeNPointCrossoverSettings::EQUIDISTANT_WIDTH)
                    {
                        for (std::size_t point = 1; point <= points; ++point)
                        {
                            positions.push_back(width * point / (points + 1));
                        }
                    }
                    else
                    {
                        const auto spare = width - (points + 1) * minimum;
                        positions.resize(spare + points);
                        std::iota(positions.begin(), positions.end(), std::size_t{0});
                        geneial::utility::Random::shuffle(positions.begin(), positions.end());
                        positions.resize(points);
                        std::sort(positions.begin(), positions.end());
                        for (std::size_t point = 0; point < points; ++point)
                        {
                            positions[point] += minimum * (point + 1) - point;
                        }
                    }
                    positions.push_back(width);
                    auto child = this->createChildCandidate();
                    auto &destination = child->getContainer();
                    destination.resize(width);
                    std::size_t begin = 0;
                    bool maternalSegment = true;
                    for (const auto end : positions)
                    {
                        const auto &source = maternalSegment ? maternal : paternal;
                        std::copy(source.begin() + begin, source.begin() + end, destination.begin() + begin);
                        begin = end;
                        maternalSegment = !maternalSegment;
                    }
                    return {child};
                }
            }
        }
    }
}
