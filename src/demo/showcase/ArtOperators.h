#pragma once

#include "ArtCanvas.h"
#include "Showcase.h"

namespace showcase::art
{
inline Chromosome::ptr child(const std::shared_ptr<Factory> &factory, std::vector<int> genome)
{
    auto result = std::dynamic_pointer_cast<Chromosome>(
        factory->createChromosome(geneial::population::chromosome::BaseChromosomeFactory<double>::LET_UNPOPULATED));
    result->setValueContainer(genome);
    return result;
}

inline std::size_t residualLocation(std::span<const int> genome, Style style, const Image &target)
{
    const auto painting = paint(genome, style, target);
    std::size_t best = 0;
    int largest = -1;
    for (unsigned int sample = 0; sample < 48; ++sample)
    {
        const auto pixel = Random::generate<std::size_t>(0, target.pixels.size() - 1);
        int error = 0;
        for (unsigned int channel = 0; channel < 3; ++channel)
        {
            const int difference = target.pixels[pixel][channel] - painting.pixels[pixel][channel];
            error += difference * difference;
        }
        if (error > largest)
        {
            best = pixel;
            largest = error;
        }
    }
    return best;
}

inline void placeShape(std::vector<int> &genome, std::size_t offset, Style style, const Image &target,
                       std::size_t pixel, int radius)
{
    const int centerX = static_cast<int>((pixel % target.width + 0.5) * 256 / target.width);
    const int centerY = static_cast<int>((pixel / target.width + 0.5) * 256 / target.height);
    if (style == Style::Voronoi)
    {
        genome[offset] = centerX;
        genome[offset + 1] = centerY;
        std::copy(target.pixels[pixel].begin(), target.pixels[pixel].end(), genome.begin() + offset + 2);
        return;
    }
    for (unsigned int vertex = 0; vertex < 3; ++vertex)
    {
        genome[offset + vertex * 2] = std::clamp(centerX + Random::generate(-radius, radius), 0, 255);
        genome[offset + vertex * 2 + 1] = std::clamp(centerY + Random::generate(-radius, radius), 0, 255);
    }
    std::copy(target.pixels[pixel].begin(), target.pixels[pixel].end(), genome.begin() + offset + 6);
    genome[offset + 9] = Random::generate(100, 230);
}

class ArtCrossover final : public geneial::operation::crossover::BaseCrossoverOperation<double>
{
    std::shared_ptr<Factory> _factory;
    Style _style;

  public:
    ArtCrossover(std::shared_ptr<Factory> factory, Style style) : _factory(std::move(factory)), _style(style) {}
    bool isSymmetric() const override
    {
        return false;
    }
    crossover_result_set
    doCrossover(const geneial::population::chromosome::BaseChromosome<double>::const_ptr &first,
                const geneial::population::chromosome::BaseChromosome<double>::const_ptr &second) const override
    {
        const auto &preferred = first->getFitness().get() >= second->getFitness().get() ? first : second;
        auto genome = values(*preferred);
        if (Random::decision(0.03))
        {
            const auto &donor = values(*(preferred == first ? second : first));
            const auto offset =
                Random::generate<std::size_t>(0, std::min(genome.size(), donor.size()) / stride(_style) - 1) *
                stride(_style);
            std::copy_n(donor.begin() + offset, stride(_style), genome.begin() + offset);
        }
        return {child(_factory, std::move(genome))};
    }
};

class ArtMutation final : public geneial::operation::mutation::BaseMutationOperation<double>
{
    std::shared_ptr<Factory> _factory;
    Style _style;
    std::shared_ptr<const Image> _target;
    unsigned int _maximumShapes;

  public:
    ArtMutation(std::shared_ptr<Factory> factory, Style style, std::shared_ptr<const Image> target,
                unsigned int maximumShapes)
        : BaseMutationOperation(geneial::operation::choosing::ChooseRandom<double>::Builder(1).create()),
          _factory(std::move(factory)), _style(style), _target(std::move(target)), _maximumShapes(maximumShapes)
    {
    }

    mutation_result_set doMutate(const mutation_result_set &chromosomes, Manager &manager) const override
    {
        struct Proposal
        {
            std::vector<int> genome;
            std::size_t selected;
        };
        std::vector<Proposal> proposals;
        proposals.reserve(chromosomes.size());
        for (const auto &chromosome : chromosomes)
        {
            auto genome = values(*chromosome);
            const auto width = stride(_style);
            auto selected = Random::generate<std::size_t>(0, genome.size() / width - 1) * width;
            const int scale = Random::generate(0, 99);
            const int radius = scale < 65 ? 3 : scale < 90 ? 14 : 60;
            if (_style == Style::Voronoi && genome.size() / width > 4 && Random::decision(0.01))
            {
                genome.erase(genome.begin() + selected, genome.begin() + selected + width);
                selected = 0;
            }
            else if (_style == Style::Voronoi && genome.size() / width < _maximumShapes && Random::decision(0.18))
            {
                const auto pixel = residualLocation(genome, _style, *_target);
                selected = genome.size();
                genome.resize(genome.size() + width);
                placeShape(genome, selected, _style, *_target, pixel, radius);
            }
            else if (Random::decision(0.08))
            {
                const auto pixel = residualLocation(genome, _style, *_target);
                placeShape(genome, selected, _style, *_target, pixel, Random::generate(8, 75));
            }
            else if (_style == Style::Voronoi)
            {
                for (unsigned int axis = 0; axis < 2; ++axis)
                    genome[selected + axis] =
                        std::clamp(genome[selected + axis] + Random::generate(-radius, radius), 0, 255);
            }
            else
            {
                const int mode = Random::generate(0, 9);
                if (mode < 5)
                {
                    const auto coordinate = Random::generate<std::size_t>(0, 5);
                    genome[selected + coordinate] =
                        std::clamp(genome[selected + coordinate] + Random::generate(-radius, radius), 0, 255);
                }
                else if (mode < 8)
                {
                    const int horizontal = Random::generate(-radius, radius),
                              vertical = Random::generate(-radius, radius);
                    for (unsigned int vertex = 0; vertex < 3; ++vertex)
                    {
                        genome[selected + vertex * 2] = std::clamp(genome[selected + vertex * 2] + horizontal, 0, 255);
                        genome[selected + vertex * 2 + 1] =
                            std::clamp(genome[selected + vertex * 2 + 1] + vertical, 0, 255);
                    }
                }
                else
                    genome[selected + 9] = std::clamp(genome[selected + 9] + Random::generate(-radius, radius), 5, 255);
            }
            proposals.push_back({std::move(genome), selected});
        }
        auto &execution = manager.getExecutionManager();
        try
        {
            for (auto &proposal : proposals)
                execution.addTask(
                    [this, &proposal]
                    {
                        if (_style == Style::Voronoi)
                            fitCellColors(proposal.genome, *_target);
                        else
                            fitTriangleColor(proposal.genome, proposal.selected, *_target);
                    });
            execution.waitForTasks();
        }
        catch (...)
        {
            execution.waitForTasks();
            throw;
        }
        mutation_result_set result;
        for (auto &proposal : proposals)
            result.push_back(child(_factory, std::move(proposal.genome)));
        return result;
    }
};
}
