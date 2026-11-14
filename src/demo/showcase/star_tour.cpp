#include "Showcase.h"

#include <cmath>
#include <random>

namespace
{

struct Point
{
    double horizontal;
    double vertical;
};

std::vector<Point> galaxy(unsigned int size)
{
    std::mt19937 generator(7);
    std::uniform_real_distribution<double> jitter(-12, 12);
    std::vector<Point> stars;
    const double pi = std::acos(-1.0);
    for (unsigned int index = 0; index < size; ++index)
    {
        const double radius = 35 + 220.0 * (index / 3) / (size / 3);
        const double angle = 2 * pi * (index % 3) / 3 + radius / 105;
        stars.push_back({450 + 1.35 * radius * std::cos(angle) + jitter(generator),
                         310 + radius * std::sin(angle) + jitter(generator)});
    }
    return stars;
}

double tourLength(std::span<const int> tour, const std::vector<Point> &stars)
{
    double distance = 0;
    for (unsigned int index = 0; index < tour.size(); ++index)
    {
        const auto &from = stars[tour[index]];
        const auto &to = stars[tour[(index + 1) % tour.size()]];
        distance += std::hypot(to.horizontal - from.horizontal, to.vertical - from.vertical);
    }
    return distance;
}

std::string render(std::span<const int> tour, const std::vector<Point> &stars)
{
    std::ostringstream svg;
    svg << showcase::svgStart("A closed starship route through a spiral galaxy")
        << "<defs><radialGradient id=\"nebula\"><stop stop-color=\"#623a91\" stop-opacity=\".48\"/>"
        << "<stop offset=\"1\" stop-color=\"#0b1023\" stop-opacity=\"0\"/></radialGradient></defs>"
        << "<ellipse cx=\"450\" cy=\"310\" rx=\"400\" ry=\"280\" fill=\"url(#nebula)\"/>";
    for (unsigned int index = 0; index < 150; ++index)
    {
        svg << "<circle cx=\"" << (index * 137 + 41) % 900 << "\" cy=\"" << (index * 73 + 19) % 620
            << "\" r=\"1\" fill=\"#6b7898\" opacity=\".35\"/>";
    }
    std::ostringstream route;
    for (unsigned int index = 0; index <= tour.size(); ++index)
    {
        const auto &star = stars[tour[index % tour.size()]];
        route << star.horizontal << ',' << star.vertical << ' ';
    }
    svg << "<polyline points=\"" << route.str()
        << "\" fill=\"none\" stroke=\"#79f5da\" stroke-opacity=\".12\" stroke-width=\"10\"/>"
        << "<polyline points=\"" << route.str()
        << "\" fill=\"none\" stroke=\"#79f5da\" stroke-width=\"1.8\" stroke-linejoin=\"round\"/>";
    for (unsigned int index = 0; index < stars.size(); ++index)
    {
        const auto &star = stars[index];
        svg << "<circle cx=\"" << star.horizontal << "\" cy=\"" << star.vertical
            << "\" r=\"4\" fill=\"#eef7ff\"><title>Star " << index << "</title></circle>";
    }
    svg << "<text x=\"32\" y=\"44\" fill=\"#7ce7d5\" font-family=\"monospace\" font-size=\"12\">" << stars.size()
        << " STARS / ONE CLOSED TOUR</text></svg>";
    return svg.str();
}

} // namespace

int main(int argc, char **argv)
{
    try
    {
        showcase::Options defaults;
        defaults.output = "star-tour";
        defaults.size = 48;
        const auto options = showcase::parseOptions(argc, argv, defaults, 8, 160);
        if (options.help)
        {
            return 0;
        }
        showcase::Random::initialize(options.seed);
        const auto stars = galaxy(options.size);
        auto evaluator = std::make_shared<showcase::VectorEvaluator>([&stars](std::span<const int> tour)
                                                                     { return -tourLength(tour, stars); });
        auto algorithm = showcase::buildAlgorithm(showcase::permutationFactory(options.size, evaluator), options, true);
        showcase::run(algorithm, options, "The shortest way through the stars",
                      "A travelling-salesperson mission through a three-armed galaxy. Every star is visited once, and "
                      "the ship returns home. "
                      "Order-preserving crossover and swap mutation turn tangled flight plans into shorter routes.",
                      [&stars](std::span<const int> tour) { return render(tour, stars); });
        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr << "star_tour: " << error.what() << '\n';
        return 1;
    }
}
