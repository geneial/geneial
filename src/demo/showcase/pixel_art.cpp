#include "Showcase.h"

#include <array>
#include <cmath>

namespace
{

const unsigned int WIDTH = 32;
const unsigned int HEIGHT = 24;

struct Color
{
    int red;
    int green;
    int blue;
};

const std::array<Color, 16> PALETTE = {{{8, 12, 31},
                                        {17, 24, 52},
                                        {31, 34, 76},
                                        {51, 38, 98},
                                        {76, 43, 119},
                                        {111, 48, 137},
                                        {156, 57, 146},
                                        {202, 74, 151},
                                        {237, 105, 157},
                                        {255, 151, 160},
                                        {255, 195, 165},
                                        {255, 229, 188},
                                        {88, 195, 188},
                                        {124, 231, 213},
                                        {179, 241, 225},
                                        {238, 248, 241}}};

std::vector<int> targetImage()
{
    std::vector<int> target(WIDTH * HEIGHT, 0);
    for (unsigned int row = 0; row < HEIGHT; ++row)
    {
        for (unsigned int column = 0; column < WIDTH; ++column)
        {
            int color = row < 16 ? std::min(5u, row / 3) : 1;
            const double horizontal = static_cast<double>(column) - 16;
            const double vertical = static_cast<double>(row) - 10;
            const double ringHeight = vertical + horizontal * 0.28;
            const double ringRadius = horizontal * horizontal / 169 + ringHeight * ringHeight / 9;
            const bool ring = ringRadius > 0.63 && ringRadius < 1.0;
            if (ring)
            {
                color = 12;
            }
            if (horizontal * horizontal + vertical * vertical < 48)
            {
                color = 6 + (row + column / 4) % 6;
            }
            if (ring && ringHeight > 0)
            {
                color = 13;
            }
            if ((column * 31 + row * 17) % 97 == 0 && row < 15 && color < 6)
            {
                color = 15;
            }
            if (row >= 19 + 2 * std::sin(column * 0.32))
            {
                color = 0;
            }
            target[row * WIDTH + column] = color;
        }
    }
    return target;
}

double error(std::span<const int> image, std::span<const int> target)
{
    double total = 0;
    for (unsigned int index = 0; index < image.size(); ++index)
    {
        const auto &actual = PALETTE[image[index]];
        const auto &expected = PALETTE[target[index]];
        const int red = actual.red - expected.red;
        const int green = actual.green - expected.green;
        const int blue = actual.blue - expected.blue;
        total += red * red + green * green + blue * blue;
    }
    return total;
}

void drawImage(std::ostream &svg, std::span<const int> image, unsigned int offset)
{
    for (unsigned int row = 0; row < HEIGHT; ++row)
    {
        for (unsigned int column = 0; column < WIDTH; ++column)
        {
            const auto &color = PALETTE[image[row * WIDTH + column]];
            svg << "<rect x=\"" << offset + column * 12 << "\" y=\"" << 155 + row * 12
                << "\" width=\"12\" height=\"12\" fill=\"rgb(" << color.red << ',' << color.green << ',' << color.blue
                << ")\"/>";
        }
    }
}

std::string render(std::span<const int> image, std::span<const int> target)
{
    std::ostringstream svg;
    svg << showcase::svgStart("Target planet and the pixel image evolved to match it")
        << "<g font-family=\"monospace\" font-size=\"12\" fill=\"#a4b2ce\">"
        << "<text x=\"40\" y=\"120\">THE TARGET</text><text x=\"476\" y=\"120\">THE EVOLVED GENOME</text></g>"
        << "<g shape-rendering=\"crispEdges\">";
    drawImage(svg, target, 40);
    drawImage(svg, image, 476);
    svg << "</g><text x=\"450\" y=\"495\" text-anchor=\"middle\" font-family=\"monospace\" font-size=\"13\" "
           "fill=\"#7ce7d5\">"
        << "768 GENES / 16 COLORS / NO GRADIENTS</text></svg>";
    return svg.str();
}

} // namespace

int main(int argc, char **argv)
{
    try
    {
        showcase::Options defaults;
        defaults.output = "pixel-art";
        defaults.generations = 3000;
        const auto options = showcase::parseOptions(argc, argv, defaults);
        if (options.help)
        {
            return 0;
        }
        showcase::Random::initialize(options.seed);
        const auto target = targetImage();
        auto evaluator = std::make_shared<showcase::VectorEvaluator>([&target](std::span<const int> image)
                                                                     { return -error(image, target); });
        showcase::Factory::Builder builder(evaluator);
        builder.getSettings().setNum(WIDTH * HEIGHT);
        builder.getSettings().setRandomMin(0);
        builder.getSettings().setRandomMax(PALETTE.size() - 1);
        auto factory = std::dynamic_pointer_cast<showcase::Factory>(builder.create());
        auto algorithm = showcase::buildAlgorithm(factory, options, false, true);
        showcase::run(algorithm, options, "Palette image",
                      "A tiny pixel painter starts with random noise and a sixteen-color palette. Each chromosome "
                      "encodes a complete image. "
                      "Four-point crossover exchanges patches; mutation changes a few pixels. Squared RGB error is the "
                      "only teacher: "
                      "no gradients, no image libraries, and no downloaded assets. More generations usually produce a "
                      "sharper planet.",
                      [&target](std::span<const int> image) { return render(image, target); });
        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr << "pixel_art: " << error.what() << '\n';
        return 1;
    }
}
