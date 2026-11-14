#include "Maze.h"
#include "Showcase.h"

namespace
{
using namespace showcase;

Chromosome::ptr walker(const std::shared_ptr<Factory> &factory, std::vector<int> genome)
{
    auto result = std::dynamic_pointer_cast<Chromosome>(
        factory->createChromosome(geneial::population::chromosome::BaseChromosomeFactory<double>::LET_UNPOPULATED));
    result->setValueContainer(genome);
    return result;
}

class RouteCrossover final : public geneial::operation::crossover::BaseCrossoverOperation<double>
{
    std::shared_ptr<Factory> _factory;
    std::shared_ptr<const maze::Maze> _maze;

  public:
    RouteCrossover(std::shared_ptr<Factory> factory, std::shared_ptr<const maze::Maze> maze)
        : _factory(std::move(factory)), _maze(std::move(maze))
    {
    }
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
        const auto &donor = values(*(preferred == first ? second : first));
        const auto cut = _maze->trace(genome).bestStep;
        if (Random::decision(0.25))
            std::copy(donor.begin() + cut, donor.end(), genome.begin() + cut);
        return {walker(_factory, std::move(genome))};
    }
};

class RouteMutation final : public geneial::operation::mutation::BaseMutationOperation<double>
{
    std::shared_ptr<Factory> _factory;
    std::shared_ptr<const maze::Maze> _maze;

  public:
    RouteMutation(std::shared_ptr<Factory> factory, std::shared_ptr<const maze::Maze> maze)
        : BaseMutationOperation(geneial::operation::choosing::ChooseRandom<double>::Builder(1).create()),
          _factory(std::move(factory)), _maze(std::move(maze))
    {
    }
    mutation_result_set doMutate(const mutation_result_set &chromosomes, Manager &) const override
    {
        mutation_result_set result;
        for (const auto &chromosome : chromosomes)
        {
            auto genome = values(*chromosome);
            auto trace = _maze->trace(genome);
            if (Random::decision(0.35))
            {
                const auto compact = _maze->compactPrefix(genome, trace);
                const auto original = genome;
                std::copy(compact.begin(), compact.end(), genome.begin());
                std::copy(original.begin() + trace.bestStep, original.end(), genome.begin() + compact.size());
                trace = _maze->trace(genome);
            }
            if (trace.cost <= _maze->distances()[0])
            {
                result.push_back(walker(_factory, std::move(genome)));
                continue;
            }
            const bool revisit = trace.remaining == 0 || Random::decision(0.15);
            const auto lastMutable = trace.remaining == 0 ? trace.bestStep - 1 : trace.bestStep;
            const auto cut = revisit ? Random::generate(0u, lastMutable) : trace.bestStep;
            auto cell = trace.positions[cut];
            auto previous = cut ? trace.positions[cut - 1] : cell;
            const auto end = std::min(genome.size(), cut + std::size_t{6});
            for (std::size_t step = cut; step < end; ++step)
            {
                std::vector<int> choices;
                for (int direction = 0; direction < 4; ++direction)
                {
                    const auto next = _maze->advance(cell, direction);
                    if (next != cell && next != previous)
                        choices.push_back(direction);
                }
                if (choices.empty())
                    for (int direction = 0; direction < 4; ++direction)
                        if (_maze->advance(cell, direction) != cell)
                            choices.push_back(direction);
                const auto direction = choices[Random::generate<std::size_t>(0, choices.size() - 1)];
                genome[step] = direction;
                previous = cell;
                cell = _maze->advance(cell, direction);
            }
            result.push_back(walker(_factory, std::move(genome)));
        }
        return result;
    }
};

std::string render(const maze::Maze &maze, std::span<const int> genome)
{
    const auto trace = maze.trace(genome);
    const double tile = 540.0 / maze.size();
    std::ostringstream svg;
    svg << svgStart("A genetic route through a seeded random maze", 660, 660) << "<g transform='translate(60 70)'>";
    for (unsigned int cell = 0; cell < maze.walls().size(); ++cell)
    {
        const double left = cell % maze.size() * tile, top = cell / maze.size() * tile;
        const auto line = [&](double firstX, double firstY, double lastX, double lastY)
        {
            svg << "<path d='M" << firstX << ' ' << firstY << 'L' << lastX << ' ' << lastY
                << "' stroke='#60718f' stroke-width='1.6'/>";
        };
        if (maze.walls()[cell] & 1)
            line(left, top, left + tile, top);
        if (maze.walls()[cell] & 8)
            line(left, top, left, top + tile);
        if (cell / maze.size() + 1 == maze.size())
            line(left, top + tile, left + tile, top + tile);
        if (cell % maze.size() + 1 == maze.size())
            line(left + tile, top, left + tile, top + tile);
    }
    svg << "<polyline fill='none' stroke='#8cebd4' stroke-width='" << tile * 0.24
        << "' stroke-linecap='round' stroke-linejoin='round' points='";
    for (unsigned int step = 0; step <= trace.bestStep; ++step)
    {
        const auto cell = trace.positions[step];
        svg << (cell % maze.size() + 0.5) * tile << ',' << (cell / maze.size() + 0.5) * tile << ' ';
    }
    svg << "'/><circle cx='" << tile / 2 << "' cy='" << tile / 2 << "' r='" << tile * 0.3 << "' fill='#8cebd4'/>"
        << "<circle cx='" << 540 - tile / 2 << "' cy='" << 540 - tile / 2 << "' r='" << tile * 0.3
        << "' fill='#f2bb68'/></g>"
        << "<text x='60' y='36' fill='#8cebd4' font-family='monospace' font-size='14'>"
        << (trace.remaining == 0 ? "EXIT FOUND" : "SEARCHING") << " / " << trace.remaining
        << " CORRIDOR STEPS TO EXIT</text></svg>";
    return svg.str();
}
}

int main(int argc, char **argv)
{
    try
    {
        Options defaults;
        defaults.output = "maze";
        defaults.size = 18;
        defaults.generations = 1000;
        const auto options = parseOptions(argc, argv, defaults, 4, 32);
        if (options.help)
            return 0;
        const auto maze = std::make_shared<const showcase::maze::Maze>(options.size, options.seed);
        const auto length = options.size * options.size * 2;
        Random::initialize(options.seed);
        auto evaluator = std::make_shared<VectorEvaluator>([maze](std::span<const int> genome)
                                                           { return -maze->trace(genome).cost; });
        Factory::Builder factoryBuilder(evaluator);
        factoryBuilder.getSettings().setNum(length);
        factoryBuilder.getSettings().setRandomMin(0);
        factoryBuilder.getSettings().setRandomMax(3);
        const auto factory = std::dynamic_pointer_cast<Factory>(factoryBuilder.create());
        geneial::algorithm::SteadyStateAlgorithm<double>::Builder builder;
        builder.setChromosomeFactory(factory);
        auto stopping = std::make_shared<geneial::algorithm::stopping_criteria::CombinedCriterion<double>>();
        stopping->add(decltype(stopping)::element_type::INIT,
                      std::make_shared<geneial::algorithm::stopping_criteria::MaxGenerationCriterion<double>>(
                          options.generations));
        stopping->add(decltype(stopping)::element_type::OR,
                      std::make_shared<geneial::algorithm::stopping_criteria::FitnessValueReachedCriterion<double>>(
                          -static_cast<double>(maze->distances()[0])));
        builder.setStoppingCriterion(stopping);
        builder.setCrossoverOperation(std::make_shared<RouteCrossover>(factory, maze));
        builder.setMutationOperation(std::make_shared<RouteMutation>(factory, maze));
        geneial::operation::selection::FitnessProportionalSelection<double>::Builder selection;
        selection.getSettings().setNumberOfParents(20);
        selection.getSettings().setNumberSelectBest(4);
        builder.setSelectionOperation(selection.create());
        geneial::operation::coupling::RandomCouplingOperation<double>::Builder coupling;
        coupling.getSettings().setNumberOfOffspring(40);
        builder.setCouplingOperation(coupling.create());
        const auto algorithm = builder.create();
        algorithm->getPopulationSettings().setMaxChromosomes(options.population);
        run(algorithm, options, "Random maze",
            "A fresh seeded maze, move-sequence genomes, corridor-aware mutation and loop-erasure repair. A reverse "
            "distance field guides fitness; no solution route is injected. Search continues until the shortest-route "
            "bound or the generation budget. Mint is the start; amber is the exit.",
            [maze](std::span<const int> genome) { return render(*maze, genome); });
        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr << "maze: " << error.what() << '\n';
        return 1;
    }
}
