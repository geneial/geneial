#pragma once

#include "Image.h"
#include <cstdint>
#include <functional>
#include <iomanip>
#include <numeric>

namespace showcase::art
{
enum class Style
{
    Voronoi,
    Triangles
};

struct Point
{
    double horizontal;
    double vertical;
};

inline std::size_t stride(Style style)
{
    return style == Style::Voronoi ? 5 : 10;
}

inline std::string color(std::span<const int> channels)
{
    return "rgb(" + std::to_string(channels[0]) + "," + std::to_string(channels[1]) + "," +
           std::to_string(channels[2]) + ")";
}

inline std::vector<Point> cellPolygon(std::span<const int> genome, std::size_t offset)
{
    std::vector<Point> polygon{{0, 0}, {256, 0}, {256, 256}, {0, 256}};
    for (std::size_t other = 0; other < genome.size() && !polygon.empty(); other += 5)
    {
        if (other == offset)
            continue;
        const double normalX = genome[other] - genome[offset];
        const double normalY = genome[other + 1] - genome[offset + 1];
        if (normalX == 0 && normalY == 0)
        {
            if (other < offset)
                polygon.clear();
            continue;
        }
        const double boundary = (genome[other] * genome[other] + genome[other + 1] * genome[other + 1] -
                                 genome[offset] * genome[offset] - genome[offset + 1] * genome[offset + 1]) /
                                2.0;
        const auto distance = [&](Point point)
        { return point.horizontal * normalX + point.vertical * normalY - boundary; };
        std::vector<Point> clipped;
        clipped.reserve(polygon.size() + 1);
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
    return polygon;
}

template <typename Visitor> void scanPolygon(std::span<const Point> polygon, const Image &canvas, Visitor visit)
{
    if (polygon.size() < 3)
        return;
    double area = 0, minimumRow = 256, maximumRow = 0;
    for (std::size_t vertex = 0; vertex < polygon.size(); ++vertex)
    {
        const auto first = polygon[vertex], second = polygon[(vertex + 1) % polygon.size()];
        area += first.horizontal * second.vertical - second.horizontal * first.vertical;
        minimumRow = std::min(minimumRow, first.vertical);
        maximumRow = std::max(maximumRow, first.vertical);
    }
    if (std::abs(area) < 1e-9)
        return;
    const int firstRow = std::max(0, static_cast<int>(std::ceil(minimumRow * canvas.height / 256 - 0.5 - 1e-9)));
    const int lastRow = std::min(static_cast<int>(canvas.height) - 1,
                                 static_cast<int>(std::floor(maximumRow * canvas.height / 256 - 0.5 + 1e-9)));
    for (int row = firstRow; row <= lastRow; ++row)
    {
        const double height = (row + 0.5) * 256 / canvas.height;
        double left = 256, right = 0;
        bool intersects = false;
        for (std::size_t vertex = 0; vertex < polygon.size(); ++vertex)
        {
            const auto first = polygon[vertex], second = polygon[(vertex + 1) % polygon.size()];
            if (height < std::min(first.vertical, second.vertical) - 1e-9 ||
                height > std::max(first.vertical, second.vertical) + 1e-9)
                continue;
            if (first.vertical == second.vertical)
            {
                left = std::min({left, first.horizontal, second.horizontal});
                right = std::max({right, first.horizontal, second.horizontal});
            }
            else
            {
                const double horizontal = first.horizontal + (height - first.vertical) *
                                                                 (second.horizontal - first.horizontal) /
                                                                 (second.vertical - first.vertical);
                left = std::min(left, horizontal);
                right = std::max(right, horizontal);
            }
            intersects = true;
        }
        if (!intersects)
            continue;
        const int firstColumn = std::max(0, static_cast<int>(std::ceil(left * canvas.width / 256 - 0.5 - 1e-9)));
        const int lastColumn = std::min(static_cast<int>(canvas.width) - 1,
                                        static_cast<int>(std::floor(right * canvas.width / 256 - 0.5 + 1e-9)));
        if (firstColumn <= lastColumn)
            visit(static_cast<unsigned int>(row), static_cast<unsigned int>(firstColumn),
                  static_cast<unsigned int>(lastColumn + 1));
    }
}

inline std::array<Point, 3> triangle(std::span<const int> gene)
{
    return {{{static_cast<double>(gene[0]), static_cast<double>(gene[1])},
             {static_cast<double>(gene[2]), static_cast<double>(gene[3])},
             {static_cast<double>(gene[4]), static_cast<double>(gene[5])}}};
}

inline std::vector<std::size_t> cellOwners(std::span<const int> genome, const Image &target)
{
    if (genome.empty() || genome.size() % 5)
        throw std::invalid_argument("Malformed Voronoi genome");
    struct Segment
    {
        std::size_t site;
        double intercept;
        double start;
    };
    std::vector<std::size_t> ordered(genome.size() / 5);
    std::iota(ordered.begin(), ordered.end(), 0);
    std::stable_sort(ordered.begin(), ordered.end(),
                     [&](auto first, auto second) { return genome[first * 5] < genome[second * 5]; });
    std::vector<std::size_t> owners(target.pixels.size(), 0);
    std::vector<Segment> envelope;
    envelope.reserve(ordered.size());
    for (unsigned int row = 0; row < target.height; ++row)
    {
        envelope.clear();
        const double vertical = (row + 0.5) * 256 / target.height;
        for (const auto index : ordered)
        {
            const auto site = index * 5;
            const double difference = vertical - genome[site + 1];
            const double intercept = genome[site] * genome[site] + difference * difference;
            if (!envelope.empty() && genome[envelope.back().site] == genome[site])
            {
                const auto &previous = envelope.back();
                if (previous.intercept < intercept || (previous.intercept == intercept && previous.site < site))
                    continue;
                envelope.pop_back();
            }
            double start = -std::numeric_limits<double>::infinity();
            while (!envelope.empty())
            {
                const auto &previous = envelope.back();
                start = (intercept - previous.intercept) / (2.0 * (genome[site] - genome[previous.site]));
                if (start >= previous.start - 1e-9)
                    break;
                envelope.pop_back();
            }
            if (envelope.empty())
                start = -std::numeric_limits<double>::infinity();
            envelope.push_back({site, intercept, start});
        }
        std::size_t active = 0;
        for (unsigned int column = 0; column < target.width; ++column)
        {
            const double horizontal = (column + 0.5) * 256 / target.width;
            const auto distance = [&](std::size_t site)
            {
                const double horizontalDifference = horizontal - genome[site];
                const double verticalDifference = vertical - genome[site + 1];
                return horizontalDifference * horizontalDifference + verticalDifference * verticalDifference;
            };
            auto best = envelope[active].site;
            double nearest = distance(best);
            while (active + 1 < envelope.size())
            {
                const auto candidate = envelope[active + 1].site;
                const double nextDistance = distance(candidate);
                if (nextDistance > distance(envelope[active].site) + 1e-9)
                    break;
                ++active;
                if (nextDistance < nearest - 1e-9 || (std::abs(nextDistance - nearest) <= 1e-9 && candidate < best))
                {
                    nearest = nextDistance;
                    best = candidate;
                }
            }
            owners[row * target.width + column] = best;
        }
    }
    return owners;
}

inline Image paint(std::span<const int> genome, Style style, const Image &target)
{
    if (genome.empty() || genome.size() % stride(style))
        throw std::invalid_argument("Malformed art genome");
    Image image{target.width, target.height, std::vector<Pixel>(target.pixels.size(), Pixel{24, 24, 24})};
    if (style == Style::Voronoi)
    {
        const auto owners = cellOwners(genome, target);
        for (std::size_t pixel = 0; pixel < owners.size(); ++pixel)
            image.pixels[pixel] = {genome[owners[pixel] + 2], genome[owners[pixel] + 3], genome[owners[pixel] + 4]};
    }
    else
        for (std::size_t offset = 0; offset < genome.size(); offset += 10)
        {
            const auto gene = genome.subspan(offset, 10);
            if (gene[9] == 0)
                continue;
            scanPolygon(triangle(gene), image,
                        [&](unsigned int row, unsigned int first, unsigned int last)
                        {
                            for (auto column = first; column < last; ++column)
                                for (unsigned int channel = 0; channel < 3; ++channel)
                                {
                                    auto &value = image.pixels[row * image.width + column][channel];
                                    value = (value * (255 - gene[9]) + gene[6 + channel] * gene[9] + 127) / 255;
                                }
                        });
        }
    return image;
}

inline void fitCellColors(std::vector<int> &genome, const Image &target)
{
    const auto owners = cellOwners(genome, target);
    std::vector<std::array<std::uint64_t, 4>> sums(genome.size() / 5);
    for (std::size_t pixel = 0; pixel < owners.size(); ++pixel)
    {
        auto &sum = sums[owners[pixel] / 5];
        for (unsigned int channel = 0; channel < 3; ++channel)
            sum[channel] += target.pixels[pixel][channel];
        ++sum[3];
    }
    for (std::size_t site = 0; site < sums.size(); ++site)
        if (sums[site][3])
            for (unsigned int channel = 0; channel < 3; ++channel)
                genome[site * 5 + 2 + channel] =
                    static_cast<int>((sums[site][channel] + sums[site][3] / 2) / sums[site][3]);
}

inline void fitTriangleColor(std::vector<int> &genome, std::size_t selected, const Image &target)
{
    const auto original = paint(genome, Style::Triangles, target);
    std::vector<double> influence(target.pixels.size(), 0);
    for (std::size_t offset = selected; offset < genome.size(); offset += 10)
    {
        const double alpha = genome[offset + 9] / 255.0;
        if (alpha == 0)
            continue;
        scanPolygon(triangle(std::span<const int>(genome).subspan(offset, 10)), target,
                    [&](unsigned int row, unsigned int first, unsigned int last)
                    {
                        for (auto column = first; column < last; ++column)
                        {
                            auto &weight = influence[row * target.width + column];
                            weight = offset == selected ? alpha : weight * (1 - alpha);
                        }
                    });
    }
    double denominator = 0;
    std::array<double, 3> numerator{};
    for (std::size_t pixel = 0; pixel < influence.size(); ++pixel)
    {
        const double weight = influence[pixel];
        denominator += weight * weight;
        for (unsigned int channel = 0; channel < 3; ++channel)
            numerator[channel] += weight * (target.pixels[pixel][channel] - original.pixels[pixel][channel] +
                                            weight * genome[selected + 6 + channel]);
    }
    if (denominator > 1e-9)
        for (unsigned int channel = 0; channel < 3; ++channel)
            genome[selected + 6 + channel] =
                std::clamp(static_cast<int>(std::lround(numerator[channel] / denominator)), 0, 255);
}

inline std::string vectorPainting(std::span<const int> genome, Style style)
{
    std::ostringstream svg;
    svg << std::fixed << std::setprecision(3) << "<rect width='256' height='256' fill='rgb(24,24,24)'/>";
    for (std::size_t offset = 0; offset < genome.size(); offset += stride(style))
    {
        const auto gene = genome.subspan(offset, stride(style));
        std::vector<Point> polygon;
        if (style == Style::Triangles)
        {
            const auto vertices = triangle(gene);
            polygon.assign(vertices.begin(), vertices.end());
        }
        else
            polygon = cellPolygon(genome, offset);
        if (polygon.empty())
            continue;
        svg << "<polygon fill='" << color(gene.subspan(style == Style::Triangles ? 6 : 2)) << "'";
        if (style == Style::Triangles)
            svg << " fill-opacity='" << gene[9] / 255.0 << "'";
        svg << " points='";
        for (const auto point : polygon)
            svg << point.horizontal << ',' << point.vertical << ' ';
        svg << "'/>";
    }
    return svg.str();
}
}
