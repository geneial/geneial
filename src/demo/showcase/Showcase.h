#pragma once

#include <geneial/algorithm/SteadyStateAlgorithm.h>
#include <geneial/algorithm/criteria/CombinedCriterion.h>
#include <geneial/algorithm/criteria/FitnessValueReachedCriterion.h>
#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>
#include <geneial/algorithm/observer/AlgorithmObserver.h>
#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/operations/crossover/MultiValuePermutationCrossover.h>
#include <geneial/core/operations/mutation/ValueSwapMutationOperation.h>
#include <geneial/core/operations/selection/FitnessProportionalSelection.h>
#include <geneial/core/population/builder/PermutationChromosomeFactory.h>
#include <span>

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace showcase
{

using Chromosome = geneial::population::chromosome::MultiValueChromosome<int, double>;
using Factory = geneial::population::chromosome::MultiValueChromosomeFactory<int, double>;
using Manager = geneial::population::management::BaseManager<double>;
using Algorithm = geneial::algorithm::BaseGeneticAlgorithm<double>;
using Random = geneial::utility::Random;
using Renderer = std::function<std::string(const std::vector<int> &)>;

struct Options
{
    unsigned int generations = 2000;
    unsigned int population = 128;
    std::uint32_t seed = 42;
    unsigned int size = 0;
    std::string output;
    bool help = false;
};

inline unsigned int parseNumber(const std::string &value, unsigned int min, unsigned int max)
{
    if (value.empty() || value.find_first_not_of("0123456789") != std::string::npos)
    {
        throw std::invalid_argument("Expected an unsigned integer, got: " + value);
    }
    const auto number = std::stoull(value);
    if (number < min || number > max)
    {
        throw std::invalid_argument("Value out of range: " + value);
    }
    return static_cast<unsigned int>(number);
}

inline Options parseOptions(int argc, char **argv, Options options, unsigned int minSize = 0, unsigned int maxSize = 0)
{
    for (int index = 1; index < argc; ++index)
    {
        const std::string name(argv[index]);
        if (name == "--help")
        {
            options.help = true;
            continue;
        }
        if (name != "--seed" && name != "--generations" && name != "--population" && name != "--output" &&
            !(name == "--size" && maxSize > 0))
        {
            throw std::invalid_argument("Unknown option: " + name);
        }
        if (++index == argc)
        {
            throw std::invalid_argument("Missing value for " + name);
        }
        const std::string value(argv[index]);
        if (name == "--seed")
        {
            options.seed = parseNumber(value, 0, UINT32_MAX);
        }
        else if (name == "--generations")
        {
            options.generations = parseNumber(value, 1, 100000);
        }
        else if (name == "--population")
        {
            options.population = parseNumber(value, 20, 2000);
        }
        else if (name == "--size")
        {
            options.size = parseNumber(value, minSize, maxSize);
        }
        else
        {
            if (value.empty())
            {
                throw std::invalid_argument("Output prefix cannot be empty");
            }
            options.output = value;
        }
    }
    if (options.help)
    {
        std::cout << "Usage: " << argv[0] << " [--seed N] [--generations 1..100000]\n"
                  << "  [--population 20..2000] [--output PATH_PREFIX]";
        if (maxSize > 0)
        {
            std::cout << " [--size " << minSize << ".." << maxSize << "]";
        }
        std::cout << "\nWrites PATH_PREFIX.html, .svg, and .csv. Parent directories must exist.\n";
    }
    return options;
}

inline const std::vector<int> &values(const geneial::population::chromosome::BaseChromosome<double> &chromosome)
{
    return dynamic_cast<const Chromosome &>(chromosome).getContainer();
}

using VectorEvaluator = geneial::FunctionFitnessEvaluator<int, double>;

inline std::shared_ptr<Factory> permutationFactory(unsigned int size, const std::shared_ptr<VectorEvaluator> &evaluator)
{
    geneial::population::chromosome::PermutationChromosomeFactory<int, double>::Builder builder(evaluator);
    builder.getSettings().setNum(size);
    builder.getSettings().setPermutationRoundsMin(size);
    builder.getSettings().setPermutationRoundsMax(size * 3);
    return std::dynamic_pointer_cast<Factory>(builder.create());
}

inline std::shared_ptr<Algorithm> buildAlgorithm(const std::shared_ptr<Factory> &factory, const Options &options,
                                                 bool permutation, bool stopAtZero = false)
{
    using namespace geneial::algorithm::stopping_criteria;
    geneial::algorithm::SteadyStateAlgorithm<double>::Builder builder;
    builder.setChromosomeFactory(factory);
    auto stopping = std::make_shared<CombinedCriterion<double>>();
    stopping->add(CombinedCriterion<double>::INIT,
                  std::make_shared<MaxGenerationCriterion<double>>(options.generations));
    if (stopAtZero)
    {
        stopping->add(CombinedCriterion<double>::OR, std::make_shared<FitnessValueReachedCriterion<double>>(0));
    }
    builder.setStoppingCriterion(stopping);

    geneial::operation::selection::FitnessProportionalSelection<double>::Builder selection;
    selection.getSettings().setNumberOfParents(20);
    selection.getSettings().setNumberSelectBest(2);
    builder.setSelectionOperation(selection.create());

    geneial::operation::coupling::RandomCouplingOperation<double>::Builder coupling;
    coupling.getSettings().setNumberOfOffspring(40);
    builder.setCouplingOperation(coupling.create());

    const auto chooser = geneial::operation::choosing::ChooseRandom<double>::Builder(0.8).create();
    if (permutation)
    {
        builder.setCrossoverOperation(
            geneial::operation::crossover::MultiValuePermutationCrossover<int, double>::Builder(factory).create());
        geneial::operation::mutation::ValueSwapMutationOperation<int, double>::Builder mutation(factory);
        mutation.getSettings().setMinimumPointsToMutate(1);
        mutation.getSettings().setMaximumPointsToMutate(2);
        mutation.setChoosingOperation(chooser);
        builder.setMutationOperation(mutation.create());
    }
    else
    {
        geneial::operation::crossover::MultiValueChromosomeNPointCrossover<int, double>::Builder crossover(factory);
        crossover.getCrossoverSettings().setCrossOverPoints(4);
        builder.setCrossoverOperation(crossover.create());
        geneial::operation::mutation::UniformMutationOperation<int, double>::Builder mutation(factory);
        mutation.getSettings().setMinimumPointsToMutate(1);
        mutation.getSettings().setMaximumPointsToMutate(4);
        mutation.setChoosingOperation(chooser);
        builder.setMutationOperation(mutation.create());
    }
    auto algorithm = builder.create();
    algorithm->getPopulationSettings().setMaxChromosomes(options.population);
    return algorithm;
}

inline std::string escapeXml(const std::string &text)
{
    std::string escaped;
    for (const char character : text)
    {
        switch (character)
        {
        case '&':
            escaped += "&amp;";
            break;
        case '<':
            escaped += "&lt;";
            break;
        case '>':
            escaped += "&gt;";
            break;
        case '"':
            escaped += "&quot;";
            break;
        case '\'':
            escaped += "&#39;";
            break;
        default:
            escaped += character;
        }
    }
    return escaped;
}

inline std::string svgStart(const std::string &title, unsigned int width = 900, unsigned int height = 620)
{
    std::ostringstream svg;
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 " << width << ' ' << height
        << "\" role=\"img\" aria-label=\"" << escapeXml(title) << "\"><title>" << escapeXml(title)
        << "</title><rect width=\"100%\" height=\"100%\" rx=\"18\" fill=\"#0b1023\"/>";
    return svg.str();
}

inline void writeFile(const std::string &path, const std::string &contents)
{
    std::ofstream stream(path.c_str(), std::ios::binary);
    stream << contents;
    stream.close();
    if (!stream)
    {
        throw std::runtime_error("Cannot write " + path + "; check that its parent directory exists and is writable");
    }
}

struct Frame
{
    unsigned int generation;
    double fitness;
    std::string svg;
};

class ReplayObserver : public geneial::algorithm::AlgorithmObserver<double>
{
  private:
    Renderer _render;
    unsigned int _stride;
    std::vector<Frame> _frames;
    std::vector<std::pair<unsigned int, double>> _history;

    void capture(Manager &manager)
    {
        const auto generation = manager.getPopulation().getAge();
        const auto best = manager.getHighestFitnessChromosome();
        _history.emplace_back(generation, best->getFitness().get());
        if (generation == 0 || generation % _stride == 0)
        {
            _frames.push_back({generation, best->getFitness().get(), _render(values(*best))});
        }
    }

  public:
    ReplayObserver(Renderer renderer, unsigned int generations)
        : _render(renderer), _stride(std::max(1u, generations / 80))
    {
        addSubscribedEvent(BEFORE_GENERATION);
        addSubscribedEvent(AFTER_GENERATION);
    }

    void updateBeforeGeneration(Manager &manager) override
    {
        if (_history.empty())
        {
            capture(manager);
        }
    }

    void updateAfterGeneration(Manager &manager) override
    {
        capture(manager);
    }

    void save(const Options &options, const std::string &title, const std::string &description,
              const Algorithm &algorithm)
    {
        const auto generation = _history.empty() ? 0u : _history.back().first;
        const auto best = algorithm.getHighestFitnessChromosome();
        if (_frames.empty() || _frames.back().generation != generation)
        {
            _frames.push_back({generation, best->getFitness().get(), _render(values(*best))});
        }
        if (_history.empty())
        {
            _history.emplace_back(0, best->getFitness().get());
        }
        std::ostringstream csv;
        csv << "generation,fitness\n" << std::setprecision(17);
        for (const auto &point : _history)
        {
            csv << point.first << ',' << point.second << '\n';
        }

        std::ostringstream html;
        html
            << R"HTML(<!doctype html><html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1"><title>)HTML"
            << escapeXml(title) << R"HTML( · GeneiAL</title><style>
:root{color-scheme:dark;font-family:system-ui,sans-serif;background:#070b16;color:#eef3ff}*{box-sizing:border-box}
body{margin:0;padding:48px 24px}main{max-width:1060px;margin:auto}.eyebrow{font-size:12px;letter-spacing:.22em;color:#7ce7d5}
h1{font-size:clamp(36px,6vw,64px);letter-spacing:-.06em;margin:14px 0}p{color:#a4b2ce;line-height:1.7;max-width:760px}
.stats{display:flex;gap:32px;flex-wrap:wrap;margin:26px 0}.stats strong{font-size:24px;display:block;font-variant-numeric:tabular-nums}
.stats span{color:#8f9fbc;font-size:12px}.panel{border:1px solid #26314b;border-radius:22px;background:#0b1023;overflow:hidden}
#stage>svg{width:100%;display:block;max-height:70vh}.controls{padding:20px;display:flex;align-items:center;gap:18px;border-top:1px solid #26314b}
button{background:#7ce7d5;color:#071a1a;border:0;border-radius:8px;padding:10px 20px;cursor:pointer;font-weight:700;min-width:100px}
button:focus-visible,input:focus-visible{outline:3px solid #e7b5ff;outline-offset:4px}button:disabled{opacity:.5;cursor:default}
input{width:100%;accent-color:#7ce7d5;min-width:0}output{font-size:12px;color:#a4b2ce;white-space:nowrap;font-variant-numeric:tabular-nums}
.chart{margin-top:24px;padding:22px}.chart svg{width:100%;height:140px;display:block}.chart p{font-size:12px;margin:0}
footer{color:#71819f;font-size:12px;margin:28px 0}#status{min-height:1.4em;font-size:14px;color:#7ce7d5}
@media(max-width:600px){body{padding:24px 12px}.controls{gap:10px;padding:12px}button{min-width:78px;padding:10px}.stats{gap:20px}}
</style></head><body><main><div class="eyebrow">GENEIAL / EVOLUTION IN MOTION</div><h1>)HTML"
            << escapeXml(title) << "</h1><p>" << escapeXml(description) << "</p><div class=\"stats\">"
            << "<div><strong>" << options.seed << "</strong><span>random seed</span></div>"
            << "<div><strong>" << generation << "</strong><span>generations evolved</span></div>"
            << "<div><strong>" << options.population << "</strong><span>population</span></div>"
            << "<div><strong>" << std::fixed << std::setprecision(2) << -best->getFitness().get()
            << "</strong><span>final cost · lower is better</span></div></div>"
            << "<div class=\"panel\"><div id=\"stage\"></div><div class=\"controls\">"
            << "<button id=\"play\" type=\"button\">Replay</button><input id=\"scrubber\" type=\"range\" min=\"0\" "
               "max=\""
            << _frames.size() - 1 << "\" value=\"" << _frames.size() - 1
            << "\" aria-label=\"Evolution generation\"><output id=\"generation\" for=\"scrubber\"></output></div></div>"
            << "<p id=\"status\" role=\"status\"></p><div class=\"panel chart\"><p>Best fitness / higher is better</p>"
            << "<svg viewBox=\"0 0 1000 140\" preserveAspectRatio=\"none\" role=\"img\" aria-label=\"Best fitness over "
               "generations\">"
            << "<path d=\"M0 130H1000\" stroke=\"#26314b\"/><polyline fill=\"none\" stroke=\"#7ce7d5\" "
               "stroke-width=\"2\" points=\"";
        const double initial = _history.front().second;
        const double improvement = std::max(1e-9, _history.back().second - initial);
        for (const auto &point : _history)
        {
            html << (1000.0 * point.first / std::max(1u, generation)) << ','
                 << (130 - 120 * (point.second - initial) / improvement) << ' ';
        }
        html << "\"/></svg></div><footer>C++20 · real GeneiAL operators · no network requests or external "
                "dependencies.<br>"
             << "A heuristic search, not a guarantee of global optimality. Seeded results are repeatable within the "
                "same build and standard library.</footer>";
        for (const auto &frame : _frames)
        {
            html << "<template class=\"frame\" data-generation=\"" << frame.generation << "\" data-cost=\""
                 << -frame.fitness << "\">" << frame.svg << "</template>";
        }
        html << R"HTML(<script>
'use strict';
const frames=Array.from(document.querySelectorAll('.frame'));
const slider=document.querySelector('#scrubber');
const stage=document.querySelector('#stage');
const play=document.querySelector('#play');
let timer=null;
function pause(){clearInterval(timer);timer=null;play.textContent='Replay';}
function show(){const frame=frames[Number(slider.value)];stage.replaceChildren(frame.content.cloneNode(true));
document.querySelector('#generation').textContent='Gen '+frame.dataset.generation;
document.querySelector('#status').textContent='Cost '+frame.dataset.cost+' · '+(Number(slider.value)+1)+' / '+frames.length+' keyframes';}
slider.addEventListener('input',()=>{pause();show();});
play.addEventListener('click',()=>{if(timer){pause();return;}if(Number(slider.value)===frames.length-1){slider.value=0;show();}
play.textContent='Pause';timer=setInterval(()=>{slider.value=Number(slider.value)+1;show();if(Number(slider.value)===frames.length-1){pause();}},100);});
document.addEventListener('visibilitychange',()=>{if(document.hidden){pause();}});
if(frames.length===1){play.disabled=true;slider.disabled=true;}
show();
</script></main></body></html>)HTML";
        writeFile(options.output + ".svg", _frames.back().svg);
        writeFile(options.output + ".csv", csv.str());
        writeFile(options.output + ".html", html.str());
        std::cout << title << ": initial cost " << -initial << ", final cost " << -best->getFitness().get()
                  << ", generations " << generation << ", seed " << options.seed << '\n'
                  << "Open " << options.output << ".html to replay evolution.\n";
    }
};

inline void run(const std::shared_ptr<Algorithm> &algorithm, const Options &options, const std::string &title,
                const std::string &description, Renderer renderer)
{
    auto observer = std::make_shared<ReplayObserver>(renderer, options.generations);
    algorithm->registerObserver(observer);
    algorithm->solve();
    observer->save(options, title, description, *algorithm);
}

} // namespace showcase
