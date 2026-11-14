#pragma once

#include "ArtOperators.h"
#include <geneial/utility/ThreadedExecutionManager.h>
#include <thread>

namespace showcase::art
{
struct ArtOptions
{
    Options evolution;
    std::string input = GENEIAL_ART_TARGET;
    unsigned int shapes = 50;
    unsigned int maximumShapes = 128;
    unsigned int resolution = 128;
    unsigned int threads = std::min(4u, std::max(1u, std::thread::hardware_concurrency()));
};

inline ArtOptions parseArtOptions(int argc, char **argv, Style style)
{
    ArtOptions options;
    options.shapes = style == Style::Voronoi ? 24 : 50;
    options.evolution.generations = 3000;
    options.evolution.population = 64;
    options.evolution.output = style == Style::Voronoi ? "voronoi_art" : "triangle_art";
    std::vector<char *> remaining{argv[0]};
    for (int index = 1; index < argc; ++index)
    {
        const std::string name(argv[index]);
        if (name == "--input" || name == "--shapes" || name == "--max-shapes" || name == "--resolution" ||
            name == "--threads")
        {
            if (++index == argc)
                throw std::invalid_argument("Missing value for " + name);
            if (name == "--input")
                options.input = argv[index];
            else if (name == "--shapes")
                options.shapes = parseNumber(argv[index], 4, 256);
            else if (name == "--max-shapes")
                options.maximumShapes = parseNumber(argv[index], 4, 256);
            else if (name == "--resolution")
                options.resolution = parseNumber(argv[index], 32, 256);
            else
                options.threads = parseNumber(argv[index], 1, 32);
        }
        else
            remaining.push_back(argv[index]);
    }
    options.maximumShapes = std::max(options.shapes, options.maximumShapes);
    options.evolution = parseOptions(static_cast<int>(remaining.size()), remaining.data(), options.evolution);
    if (options.evolution.help)
        std::cout
            << "Art options: --input target.ppm --shapes 4..256 --max-shapes 4..256 --threads 1..32\n"
            << "--resolution 32..256: longest fitness-raster edge (default 128). Reference retains up to 600 pixels.\n"
            << "Voronoi grows up to max-shapes; triangles keep N layers. Hybrid genetic search fits region colors.\n";
    return options;
}

inline std::string render(std::span<const int> genome, Style style, const Image &target, const std::string &targetUri)
{
    std::ostringstream svg;
    svg << svgStart("Target and evolving painting", 900, 700)
        << "<g fill='#a4b2ce' font-family='system-ui' font-size='15'><text x='35' y='40'>TARGET / " << target.width
        << " × " << target.height << "</text><text x='465' y='40'>EVOLVED / " << genome.size() / stride(style)
        << (style == Style::Voronoi ? " VORONOI CELLS" : " TRANSLUCENT TRIANGLES") << "</text></g>";
    const double height = std::min(600.0, 390.0 * target.height / target.width);
    const double width = height * target.width / target.height;
    svg << "<image href='" << targetUri << "' x='35' y='65' width='" << width << "' height='" << height << "'/>"
        << "<svg x='465' y='65' width='" << width << "' height='" << height
        << "' viewBox='0 0 256 256' preserveAspectRatio='none'>" << vectorPainting(genome, style) << "</svg></svg>";
    return svg.str();
}

inline int artMain(int argc, char **argv, Style style)
{
    try
    {
        const auto options = parseArtOptions(argc, argv, style);
        if (options.evolution.help)
            return 0;
        const auto reference = std::make_shared<const Image>(loadTarget(options.input));
        const auto target = std::make_shared<const Image>(resizeImage(*reference, options.resolution));
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
        coupling.getSettings().setNumberOfOffspring(24);
        builder.setCouplingOperation(coupling.create());
        builder.setCrossoverOperation(std::make_shared<ArtCrossover>(factory, style));
        builder.setMutationOperation(std::make_shared<ArtMutation>(factory, style, target, options.maximumShapes));
        const auto algorithm = builder.create();
        algorithm->getPopulationSettings().setMaxChromosomes(options.evolution.population);
        algorithm->setExecutionManager(std::make_unique<geneial::utility::ThreadedExecutionManager>(options.threads));
        for (unsigned int specimen = 0; specimen < options.evolution.population; ++specimen)
        {
            std::vector<int> genome(options.shapes * stride(style));
            for (std::size_t offset = 0; offset < genome.size(); offset += stride(style))
                placeShape(genome, offset, style, *target, Random::generate<std::size_t>(0, target->pixels.size() - 1),
                           Random::generate(25, 180));
            if (style == Style::Voronoi)
                fitCellColors(genome, *target);
            else
                for (std::size_t offset = 0; offset < genome.size(); offset += 10)
                    fitTriangleColor(genome, offset, *target);
            algorithm->getPopulation().insertChromosome(child(factory, std::move(genome)));
        }
        const auto uri = bitmapUri(*reference);
        run(algorithm, options.evolution, style == Style::Voronoi ? "Voronoi portrait" : "Triangle portrait / EvoLisa",
            style == Style::Voronoi
                ? "A hybrid genetic search grows an error-guided mosaic and fits each cell's mean color. Inspired by "
                  "Sebastian Proost's Voronoi genetic art."
                : "A hybrid genetic search combines multiscale geometry mutations, residual-guided restarts and "
                  "least-squares color fitting. Inspired by Károly Zsolnai-Fehér's parallel EvoLisa.",
            [reference, style, uri](std::span<const int> genome) { return render(genome, style, *reference, uri); });
        writeFile(options.evolution.output + ".ppm",
                  ppm(paint(values(*algorithm->getHighestFitnessChromosome()), style, *target)));
        std::cout << "Fitness: RGB mean squared error at " << target->width << " × " << target->height << "; "
                  << options.threads << " worker(s). Full-detail reference, vector output, hybrid color refinement.\n";
        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
}
