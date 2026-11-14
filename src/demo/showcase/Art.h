#pragma once

#include "Image.h"
#include "Showcase.h"
#include <geneial/utility/ThreadedExecutionManager.h>
#include <thread>

namespace showcase::art
{
enum class Style
{
    Voronoi,
    Triangles
};

struct ArtOptions
{
    Options evolution;
    std::string input = GENEIAL_ART_TARGET;
    unsigned int shapes = 50;
    unsigned int threads = std::min(4u, std::max(1u, std::thread::hardware_concurrency()));
};

inline ArtOptions parseArtOptions(int argc, char **argv, Style style)
{
    ArtOptions options;
    options.shapes = style == Style::Voronoi ? 16 : 50;
    options.evolution.generations = 3000;
    options.evolution.population = 64;
    options.evolution.output = style == Style::Voronoi ? "voronoi_art" : "triangle_art";
    std::vector<char *> remaining{argv[0]};
    for (int index = 1; index < argc; ++index)
    {
        const std::string name(argv[index]);
        if (name == "--input" || name == "--shapes" || name == "--threads")
        {
            if (++index == argc)
                throw std::invalid_argument("Missing value for " + name);
            if (name == "--input")
                options.input = argv[index];
            else if (name == "--shapes")
                options.shapes = parseNumber(argv[index], 4, 128);
            else
                options.threads = parseNumber(argv[index], 1, 32);
        }
        else
            remaining.push_back(argv[index]);
    }
    options.evolution = parseOptions(static_cast<int>(remaining.size()), remaining.data(), options.evolution);
    if (options.evolution.help)
        std::cout << "Art options: --input target.ppm --shapes 4..128 --threads 1..32\n"
                  << "PPM: P3/P6, maximum sample <=255; fitness raster limited to 40 x 60.\n"
                  << "Voronoi starts with N sites and can duplicate/delete up to 128. Triangles keep N layers.\n";
    return options;
}

inline std::size_t stride(Style style)
{
    return style == Style::Voronoi ? 5 : 10;
}

inline std::string color(std::span<const int> channels)
{
    return "rgb(" + std::to_string(channels[0]) + "," + std::to_string(channels[1]) + "," +
           std::to_string(channels[2]) + ")";
}

inline double edge(double firstX, double firstY, double secondX, double secondY, double pointX, double pointY)
{
    return (pointX - firstX) * (secondY - firstY) - (pointY - firstY) * (secondX - firstX);
}

inline Image paint(std::span<const int> genome, Style style, const Image &target)
{
    if (genome.empty() || genome.size() % stride(style))
        throw std::invalid_argument("Malformed art genome");
    Image image{target.width, target.height, std::vector<Pixel>(target.pixels.size(), Pixel{24, 24, 24})};
    if (style == Style::Voronoi)
    {
        for (unsigned int row = 0; row < image.height; ++row)
            for (unsigned int column = 0; column < image.width; ++column)
            {
                const double pointX = (column + 0.5) * 256 / image.width;
                const double pointY = (row + 0.5) * 256 / image.height;
                double nearest = std::numeric_limits<double>::max();
                std::size_t best = 0;
                for (std::size_t site = 0; site < genome.size(); site += 5)
                {
                    const double distance = std::pow(pointX - genome[site], 2) + std::pow(pointY - genome[site + 1], 2);
                    if (distance < nearest)
                    {
                        nearest = distance;
                        best = site;
                    }
                }
                image.pixels[row * image.width + column] = {genome[best + 2], genome[best + 3], genome[best + 4]};
            }
    }
    else
    {
        for (std::size_t shape = 0; shape < genome.size(); shape += 10)
        {
            const auto gene = genome.subspan(shape, 10);
            const double signedArea = edge(gene[0], gene[1], gene[2], gene[3], gene[4], gene[5]);
            if (signedArea == 0)
                continue;
            const unsigned int firstColumn = std::min({gene[0], gene[2], gene[4]}) * image.width / 256;
            const unsigned int lastColumn =
                std::min(image.width, 1 + std::max({gene[0], gene[2], gene[4]}) * image.width / 256);
            const unsigned int firstRow = std::min({gene[1], gene[3], gene[5]}) * image.height / 256;
            const unsigned int lastRow =
                std::min(image.height, 1 + std::max({gene[1], gene[3], gene[5]}) * image.height / 256);
            for (unsigned int row = firstRow; row < lastRow; ++row)
                for (unsigned int column = firstColumn; column < lastColumn; ++column)
                {
                    const double pointX = (column + 0.5) * 256 / image.width;
                    const double pointY = (row + 0.5) * 256 / image.height;
                    if (edge(gene[0], gene[1], gene[2], gene[3], pointX, pointY) * signedArea < 0 ||
                        edge(gene[2], gene[3], gene[4], gene[5], pointX, pointY) * signedArea < 0 ||
                        edge(gene[4], gene[5], gene[0], gene[1], pointX, pointY) * signedArea < 0)
                        continue;
                    auto &pixel = image.pixels[row * image.width + column];
                    for (unsigned int channel = 0; channel < 3; ++channel)
                        pixel[channel] = (pixel[channel] * (255 - gene[9]) + gene[6 + channel] * gene[9] + 127) / 255;
                }
        }
    }
    return image;
}

struct Point
{
    double horizontal;
    double vertical;
};

inline std::string vectorPainting(std::span<const int> genome, Style style)
{
    std::ostringstream svg;
    svg << std::fixed << std::setprecision(3) << "<rect width='256' height='256' fill='rgb(24,24,24)'/>";
    for (std::size_t offset = 0; offset < genome.size(); offset += stride(style))
    {
        const auto gene = genome.subspan(offset, stride(style));
        if (style == Style::Triangles)
        {
            svg << "<polygon points='" << gene[0] << ',' << gene[1] << ' ' << gene[2] << ',' << gene[3] << ' '
                << gene[4] << ',' << gene[5] << "' fill='" << color(gene.subspan(6)) << "' fill-opacity='"
                << gene[9] / 255.0 << "'/>";
            continue;
        }
        std::vector<Point> polygon{{0, 0}, {256, 0}, {256, 256}, {0, 256}};
        for (std::size_t other = 0; other < genome.size() && !polygon.empty(); other += 5)
        {
            if (other == offset)
                continue;
            const double normalX = genome[other] - gene[0];
            const double normalY = genome[other + 1] - gene[1];
            if (normalX == 0 && normalY == 0)
            {
                if (other < offset)
                    polygon.clear();
                continue;
            }
            const double boundary = (genome[other] * genome[other] + genome[other + 1] * genome[other + 1] -
                                     gene[0] * gene[0] - gene[1] * gene[1]) /
                                    2.0;
            auto distance = [&](Point point)
            { return point.horizontal * normalX + point.vertical * normalY - boundary; };
            std::vector<Point> clipped;
            for (std::size_t vertex = 0; vertex < polygon.size(); ++vertex)
            {
                const auto first = polygon[vertex];
                const auto second = polygon[(vertex + 1) % polygon.size()];
                const double firstDistance = distance(first), secondDistance = distance(second);
                if (firstDistance <= 0)
                    clipped.push_back(first);
                if ((firstDistance <= 0) != (secondDistance <= 0))
                {
                    const double fraction = firstDistance / (firstDistance - secondDistance);
                    clipped.push_back({first.horizontal + fraction * (second.horizontal - first.horizontal),
                                       first.vertical + fraction * (second.vertical - first.vertical)});
                }
            }
            polygon = std::move(clipped);
        }
        if (!polygon.empty())
        {
            svg << "<polygon fill='" << color(gene.subspan(2)) << "' points='";
            for (const auto point : polygon)
                svg << point.horizontal << ',' << point.vertical << ' ';
            svg << "'/>";
        }
    }
    return svg.str();
}

inline std::string render(std::span<const int> genome, Style style, const Image &target, const std::string &targetUri)
{
    std::ostringstream svg;
    svg << svgStart("Target and evolving painting", 900, 700)
        << "<g fill='#a4b2ce' font-family='system-ui' font-size='15'><text x='35' y='40'>TARGET / 40 × 60 MAX</text>"
        << "<text x='465' y='40'>EVOLVED / " << genome.size() / stride(style)
        << (style == Style::Voronoi ? " VORONOI CELLS" : " TRANSLUCENT TRIANGLES") << "</text></g>";
    const double height = std::min(600.0, 390.0 * target.height / target.width);
    const double width = height * target.width / target.height;
    svg << "<image href='" << targetUri << "' x='35' y='65' width='" << width << "' height='" << height << "'/>"
        << "<svg x='465' y='65' width='" << width << "' height='" << height
        << "' viewBox='0 0 256 256' preserveAspectRatio='none'>" << vectorPainting(genome, style) << "</svg></svg>";
    return svg.str();
}

inline Chromosome::ptr child(const std::shared_ptr<Factory> &factory, std::vector<int> genome)
{
    auto result = std::dynamic_pointer_cast<Chromosome>(
        factory->createChromosome(geneial::population::chromosome::BaseChromosomeFactory<double>::LET_UNPOPULATED));
    result->setValueContainer(genome);
    return result;
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
        const auto &other = preferred == first ? second : first;
        auto genome = values(*preferred);
        if (Random::decision(0.12))
        {
            const auto &donor = values(*other);
            const auto block =
                Random::generate<std::size_t>(0, std::min(genome.size(), donor.size()) / stride(_style) - 1) *
                stride(_style);
            std::copy_n(donor.begin() + block, stride(_style), genome.begin() + block);
        }
        return {child(_factory, std::move(genome))};
    }
};

class ArtMutation final : public geneial::operation::mutation::BaseMutationOperation<double>
{
    std::shared_ptr<Factory> _factory;
    Style _style;
    unsigned int _generations;

  public:
    ArtMutation(std::shared_ptr<Factory> factory, Style style, unsigned int generations)
        : BaseMutationOperation(geneial::operation::choosing::ChooseRandom<double>::Builder(1).create()),
          _factory(std::move(factory)), _style(style), _generations(generations)
    {
    }
    mutation_result_set doMutate(const mutation_result_set &chromosomes, Manager &manager) const override
    {
        mutation_result_set result;
        const int radius = std::max(3, 30 - static_cast<int>(27.0 * manager.getPopulation().getAge() / _generations));
        for (const auto &chromosome : chromosomes)
        {
            auto genome = values(*chromosome);
            const auto width = stride(_style);
            if (_style == Style::Voronoi && genome.size() / width < 128 && Random::decision(0.08))
            {
                const auto offset = Random::generate<std::size_t>(0, genome.size() / width - 1) * width;
                const std::vector<int> duplicate(genome.begin() + offset, genome.begin() + offset + width);
                genome.insert(genome.end(), duplicate.begin(), duplicate.end());
                genome[genome.size() - width] =
                    std::clamp(genome[genome.size() - width] + Random::generate(-radius, radius), 0, 255);
            }
            else if (_style == Style::Voronoi && genome.size() / width > 4 && Random::decision(0.03))
            {
                const auto offset = Random::generate<std::size_t>(0, genome.size() / width - 1) * width;
                genome.erase(genome.begin() + offset, genome.begin() + offset + width);
            }
            const auto changes = Random::generate(1, 3);
            for (int change = 0; change < changes; ++change)
            {
                auto &value = genome[Random::generate<std::size_t>(0, genome.size() - 1)];
                value = std::clamp(value + Random::generate(-radius, radius), 0, 255);
            }
            if (_style == Style::Triangles && Random::decision(0.02))
            {
                const auto first = Random::generate<std::size_t>(0, genome.size() / width - 1) * width;
                const auto second = Random::generate<std::size_t>(0, genome.size() / width - 1) * width;
                for (std::size_t index = 0; index < width; ++index)
                    std::swap(genome[first + index], genome[second + index]);
            }
            result.push_back(child(_factory, std::move(genome)));
        }
        return result;
    }
};

inline int artMain(int argc, char **argv, Style style)
{
    try
    {
        const auto options = parseArtOptions(argc, argv, style);
        if (options.evolution.help)
            return 0;
        const auto target = std::make_shared<const Image>(loadTarget(options.input));
        Random::initialize(options.evolution.seed);
        auto evaluator =
            std::make_shared<VectorEvaluator>([target, style](std::span<const int> genome)
                                              { return -meanSquaredError(paint(genome, style, *target), *target); });
        Factory::Builder factoryBuilder(evaluator);
        factoryBuilder.getSettings().setNum(options.shapes * stride(style));
        factoryBuilder.getSettings().setRandomMin(0);
        factoryBuilder.getSettings().setRandomMax(255);
        const auto factory = std::dynamic_pointer_cast<Factory>(factoryBuilder.create());
        geneial::algorithm::SteadyStateAlgorithm<double>::Builder builder;
        builder.setChromosomeFactory(factory);
        builder.setStoppingCriterion(
            std::make_shared<geneial::algorithm::stopping_criteria::MaxGenerationCriterion<double>>(
                options.evolution.generations));
        geneial::operation::selection::FitnessProportionalSelection<double>::Builder selection;
        selection.getSettings().setNumberOfParents(20);
        selection.getSettings().setNumberSelectBest(4);
        builder.setSelectionOperation(selection.create());
        geneial::operation::coupling::RandomCouplingOperation<double>::Builder coupling;
        coupling.getSettings().setNumberOfOffspring(32);
        builder.setCouplingOperation(coupling.create());
        builder.setCrossoverOperation(std::make_shared<ArtCrossover>(factory, style));
        builder.setMutationOperation(std::make_shared<ArtMutation>(factory, style, options.evolution.generations));
        const auto algorithm = builder.create();
        algorithm->getPopulationSettings().setMaxChromosomes(options.evolution.population);
        algorithm->setExecutionManager(std::make_unique<geneial::utility::ThreadedExecutionManager>(options.threads));
        for (unsigned int specimen = 0; specimen < options.evolution.population; ++specimen)
        {
            std::vector<int> genome(options.shapes * stride(style));
            for (auto &value : genome)
                value = Random::generate(0, 255);
            for (std::size_t offset = 0; offset < genome.size(); offset += stride(style))
            {
                const unsigned int centerX = style == Style::Voronoi
                                                 ? genome[offset]
                                                 : (genome[offset] + genome[offset + 2] + genome[offset + 4]) / 3;
                const unsigned int centerY = style == Style::Voronoi
                                                 ? genome[offset + 1]
                                                 : (genome[offset + 1] + genome[offset + 3] + genome[offset + 5]) / 3;
                const auto &sampled = target->at(centerX * target->width / 256, centerY * target->height / 256);
                std::copy(sampled.begin(), sampled.end(), genome.begin() + offset + (style == Style::Voronoi ? 2 : 6));
                if (style == Style::Triangles)
                    genome[offset + 9] = Random::generate(20, 160);
            }
            algorithm->getPopulation().insertChromosome(child(factory, std::move(genome)));
        }
        const auto uri = bitmapUri(*target);
        run(algorithm, options.evolution,
            style == Style::Voronoi ? "Chromatic cells" : "EvoLisa / translucent triangles",
            style == Style::Voronoi ? "A variable-length genome grows and prunes a living mosaic. Inspired by "
                                      "Sebastian Proost's Voronoi genetic art."
                                    : "A portrait emerges from overlapping translucent triangles. Inspired by Károly "
                                      "Zsolnai-Fehér's parallel EvoLisa.",
            [target, style, uri](std::span<const int> genome) { return render(genome, style, *target, uri); });
        writeFile(options.evolution.output + ".ppm",
                  ppm(paint(values(*algorithm->getHighestFitnessChromosome()), style, *target)));
        std::cout << "Fitness: RGB mean squared error; " << options.threads
                  << " worker(s). Target-informed color initialization.\n";
        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
} // namespace showcase::art
