#include "Showcase.h"

#include <cmath>

namespace
{

bool attacks(std::span<const int> board, unsigned int first, unsigned int second)
{
    return std::abs(board[first] - board[second]) == static_cast<int>(second - first);
}

unsigned int conflicts(std::span<const int> board)
{
    unsigned int count = 0;
    for (unsigned int first = 0; first < board.size(); ++first)
    {
        for (unsigned int second = first + 1; second < board.size(); ++second)
        {
            count += attacks(board, first, second);
        }
    }
    return count;
}

std::string render(std::span<const int> board)
{
    std::ostringstream svg;
    svg << showcase::svgStart("N-queens board with attacking diagonals highlighted");
    const double cell = 520.0 / board.size();
    for (unsigned int column = 0; column < board.size(); ++column)
    {
        for (unsigned int row = 0; row < board.size(); ++row)
        {
            svg << "<rect x=\"" << 190 + column * cell << "\" y=\"" << 50 + row * cell << "\" width=\"" << cell
                << "\" height=\"" << cell << "\" fill=\"" << ((column + row) % 2 ? "#18223c" : "#253251") << "\"/>";
        }
    }
    std::vector<bool> threatened(board.size(), false);
    for (unsigned int first = 0; first < board.size(); ++first)
    {
        for (unsigned int second = first + 1; second < board.size(); ++second)
        {
            if (attacks(board, first, second))
            {
                threatened[first] = threatened[second] = true;
                svg << "<line x1=\"" << 190 + (first + 0.5) * cell << "\" y1=\"" << 50 + (board[first] + 0.5) * cell
                    << "\" x2=\"" << 190 + (second + 0.5) * cell << "\" y2=\"" << 50 + (board[second] + 0.5) * cell
                    << "\" stroke=\"#fc759d\" stroke-width=\"2\" opacity=\".55\"/>";
            }
        }
    }
    for (unsigned int column = 0; column < board.size(); ++column)
    {
        svg << "<g transform=\"translate(" << 190 + (column + 0.5) * cell << ',' << 50 + (board[column] + 0.5) * cell
            << ") scale(" << cell / 32 << ")\" fill=\"" << (threatened[column] ? "#fc759d" : "#7ce7d5")
            << "\"><path d=\"M-11 -7L-7 7H7L11 -7L5 -2L0 -10L-5 -2ZM-7 9H7V12H-7Z\"/>"
            << "<circle cy=\"-12\" r=\"2\"/></g>";
    }
    svg << "<text x=\"450\" y=\"606\" text-anchor=\"middle\" fill=\"#a4b2ce\" font-family=\"monospace\" "
           "font-size=\"14\">"
        << board.size() << " QUEENS / " << conflicts(board) << " ATTACKING PAIRS</text></svg>";
    return svg.str();
}

} // namespace

int main(int argc, char **argv)
{
    try
    {
        showcase::Options defaults;
        defaults.output = "queens";
        defaults.generations = 4000;
        defaults.size = 20;
        const auto options = showcase::parseOptions(argc, argv, defaults, 4, 48);
        if (options.help)
        {
            return 0;
        }
        showcase::Random::initialize(options.seed);
        auto evaluator = std::make_shared<showcase::VectorEvaluator>(
            [](std::span<const int> board) { return -static_cast<double>(conflicts(board)); });
        auto algorithm =
            showcase::buildAlgorithm(showcase::permutationFactory(options.size, evaluator), options, true, true);
        showcase::run(algorithm, options, "A truce between queens",
                      "Place twenty queens so that none can attack another. A permutation guarantees one queen per row "
                      "and column; "
                      "evolution only has to untangle the diagonals. Pink lines expose conflicts, mint crowns mark "
                      "queens at peace. "
                      "Use --size to change the board. Search stops at zero conflicts or the generation budget.",
                      render);
        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr << "queens: " << error.what() << '\n';
        return 1;
    }
}
