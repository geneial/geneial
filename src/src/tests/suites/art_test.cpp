#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE art
#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif
#include "../../../demo/showcase/Art.h"
#include <random>

using namespace showcase::art;

BOOST_AUTO_TEST_CASE(ppm_roundtrip_comments_scaling_and_binary_whitespace)
{
    std::istringstream text("P3\n# comment\n2 1\n15\n15 0 0 # red\n0 15 0\n");
    const auto image = readPpm(text);
    BOOST_CHECK_EQUAL(image.at(0, 0)[0], 255);
    BOOST_CHECK_EQUAL(image.at(1, 0)[1], 255);
    std::istringstream roundtrip(ppm(image));
    BOOST_CHECK_EQUAL(meanSquaredError(readPpm(roundtrip), image), 0);
    std::string binary = "P6\n1 1\n255\n";
    binary += char(10);
    binary += char(32);
    binary += char(255);
    std::istringstream raster(binary);
    const auto decoded = readPpm(raster);
    BOOST_CHECK_EQUAL(decoded.pixels[0][0], 10);
    BOOST_CHECK_EQUAL(decoded.pixels[0][1], 32);
    BOOST_CHECK_EQUAL(decoded.pixels[0][2], 255);
    BOOST_CHECK(bitmapUri(image).starts_with("data:image/bmp;base64,Qk"));
}

BOOST_AUTO_TEST_CASE(ppm_rejects_malformed_and_oversized_inputs)
{
    for (const auto *input : {"P9 1 1 255", "P3 0 1 255", "P3 4097 1 255", "P3 1 1 65535", "P3 1 1 255 0 0",
                              "P6 1 1 255\n", "P3 1 1 255 256 0 0"})
    {
        std::istringstream stream(input);
        BOOST_CHECK_THROW(readPpm(stream), std::runtime_error);
    }
}

BOOST_AUTO_TEST_CASE(voronoi_duplication_is_neutral_and_ties_are_stable)
{
    Image target{2, 1, {{255, 0, 0}, {0, 0, 255}}};
    std::vector<int> genome{0, 128, 255, 0, 0, 255, 128, 0, 0, 255};
    const auto original = paint(genome, Style::Voronoi, target);
    BOOST_CHECK_EQUAL(meanSquaredError(original, target), 0);
    genome.insert(genome.end(), {0, 128, 0, 255, 0});
    BOOST_CHECK_EQUAL(meanSquaredError(paint(genome, Style::Voronoi, target), original), 0);
    BOOST_CHECK(vectorPainting(genome, Style::Voronoi).find("polygon") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(triangle_rasterization_obeys_order_alpha_and_degenerate_geometry)
{
    Image target{2, 2, std::vector<Pixel>(4)};
    const std::vector<int> triangle{0, 0, 255, 0, 0, 255, 255, 0, 0, 255};
    const auto opaque = paint(triangle, Style::Triangles, target);
    BOOST_CHECK_EQUAL(opaque.pixels.front()[0], 255);
    BOOST_CHECK_EQUAL(opaque.pixels.back()[0], 24);
    auto layers = triangle;
    layers.insert(layers.end(), {0, 0, 255, 0, 0, 255, 0, 0, 255, 128});
    const auto blended = paint(layers, Style::Triangles, target);
    BOOST_CHECK_EQUAL(blended.pixels.front()[0], 127);
    BOOST_CHECK_EQUAL(blended.pixels.front()[2], 128);
    layers.insert(layers.end(), {0, 0, 0, 0, 0, 0, 255, 255, 255, 255});
    BOOST_CHECK_EQUAL(meanSquaredError(paint(layers, Style::Triangles, target), blended), 0);
}

BOOST_AUTO_TEST_CASE(area_resize_preserves_detail_and_averages_samples)
{
    const Image source{
        4,
        2,
        {{0, 0, 0}, {100, 0, 0}, {0, 100, 0}, {0, 200, 0}, {0, 0, 100}, {100, 0, 100}, {0, 100, 100}, {0, 200, 100}}};
    const auto smaller = resizeImage(source, 2);
    BOOST_CHECK_EQUAL(smaller.width, 2);
    BOOST_CHECK_EQUAL(smaller.height, 1);
    BOOST_CHECK_EQUAL(smaller.pixels[0][0], 50);
    BOOST_CHECK_EQUAL(smaller.pixels[1][1], 150);
    BOOST_CHECK_EQUAL(smaller.pixels[0][2], 50);
    BOOST_CHECK_EQUAL(meanSquaredError(resizeImage(source, 600), source), 0);
    BOOST_CHECK_THROW(resizeImage(source, 0), std::invalid_argument);
    BOOST_CHECK_THROW(resizeImage(Image{2, 2, {}}, 2), std::invalid_argument);
    const auto reference = loadTarget(GENEIAL_ART_TARGET);
    BOOST_CHECK_EQUAL(reference.width, 396);
    BOOST_CHECK_EQUAL(reference.height, 599);
    BOOST_CHECK_EQUAL(resizeImage(reference, 128).height, 128);
}

BOOST_AUTO_TEST_CASE(scanline_cells_match_brute_force_nearest_sites)
{
    std::mt19937 generator(42);
    std::uniform_int_distribution<int> channel(0, 255);
    for (const auto dimensions : {std::pair{1u, 1u}, {37u, 19u}, {19u, 37u}})
        for (unsigned int trial = 0; trial < 80; ++trial)
        {
            Image target{dimensions.first, dimensions.second, std::vector<Pixel>(dimensions.first * dimensions.second)};
            std::vector<int> genome(5 * (trial % 4 == 0 ? 256 : 4 + trial));
            for (auto &gene : genome)
                gene = channel(generator);
            if (trial % 3 == 0)
            {
                genome[5] = genome[0];
                genome[6] = genome[1];
            }
            const auto owners = cellOwners(genome, target);
            for (unsigned int row = 0; row < target.height; ++row)
                for (unsigned int column = 0; column < target.width; ++column)
                {
                    const double pointX = (column + 0.5) * 256 / target.width;
                    const double pointY = (row + 0.5) * 256 / target.height;
                    double nearest = std::numeric_limits<double>::max();
                    std::size_t expected = 0;
                    for (std::size_t offset = 0; offset < genome.size(); offset += 5)
                    {
                        const double horizontal = pointX - genome[offset], vertical = pointY - genome[offset + 1];
                        const double distance = horizontal * horizontal + vertical * vertical;
                        if (distance < nearest)
                        {
                            nearest = distance;
                            expected = offset;
                        }
                    }
                    BOOST_REQUIRE_EQUAL(owners[row * target.width + column], expected);
                }
        }
    const Image center{1, 1, {{0, 0, 0}}};
    const std::vector<int> tie{128, 1, 255, 0, 0, 1, 128, 0, 255, 0, 255, 128, 0, 0, 255, 128, 255, 1, 2, 3};
    BOOST_CHECK_EQUAL(cellOwners(tie, center).front(), 0);
    BOOST_CHECK_THROW(cellOwners({}, center), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(scanline_triangles_match_edge_function_in_both_windings)
{
    std::mt19937 generator(7);
    std::uniform_int_distribution<int> coordinate(0, 255);
    Image target{37, 19, std::vector<Pixel>(37 * 19)};
    const auto edge = [](Point first, Point second, Point point)
    {
        return (point.horizontal - first.horizontal) * (second.vertical - first.vertical) -
               (point.vertical - first.vertical) * (second.horizontal - first.horizontal);
    };
    for (unsigned int trial = 0; trial < 200; ++trial)
    {
        std::vector<int> genome(10);
        for (auto &gene : genome)
            gene = coordinate(generator);
        genome[6] = 255;
        genome[7] = 0;
        genome[8] = 0;
        genome[9] = 255;
        const auto vertices = triangle(genome);
        const auto area = edge(vertices[0], vertices[1], vertices[2]);
        const auto raster = paint(genome, Style::Triangles, target);
        for (unsigned int row = 0; row < target.height; ++row)
            for (unsigned int column = 0; column < target.width; ++column)
            {
                const Point point{(column + 0.5) * 256 / target.width, (row + 0.5) * 256 / target.height};
                const bool inside = area != 0 && edge(vertices[0], vertices[1], point) * area >= 0 &&
                                    edge(vertices[1], vertices[2], point) * area >= 0 &&
                                    edge(vertices[2], vertices[0], point) * area >= 0;
                BOOST_REQUIRE_EQUAL(raster.at(column, row)[0], inside ? 255 : 24);
            }
        BOOST_CHECK(vectorPainting(genome, Style::Triangles).find("polygon") != std::string::npos);
    }
}

BOOST_AUTO_TEST_CASE(region_color_fitting_reduces_error_and_handles_occlusion)
{
    const Image target{2, 2, {{100, 150, 200}, {100, 150, 200}, {100, 150, 200}, {100, 150, 200}}};
    std::vector<int> sites{0, 0, 0, 0, 0, 255, 255, 255, 255, 255};
    fitCellColors(sites, target);
    BOOST_CHECK_EQUAL(meanSquaredError(paint(sites, Style::Voronoi, target), target), 0);
    std::vector<int> layers{0, 0, 255, 0, 0, 255, 0, 0, 0, 255};
    const auto before = meanSquaredError(paint(layers, Style::Triangles, target), target);
    fitTriangleColor(layers, 0, target);
    BOOST_CHECK_LT(meanSquaredError(paint(layers, Style::Triangles, target), target), before);
    BOOST_CHECK_EQUAL(layers[6], 100);
    layers.insert(layers.end(), {0, 0, 255, 0, 0, 255, 1, 2, 3, 255});
    const auto occluded = layers;
    fitTriangleColor(layers, 0, target);
    BOOST_CHECK(layers == occluded);
    layers.back() = 0;
    const auto transparent = layers;
    fitTriangleColor(layers, 10, target);
    BOOST_CHECK(layers == transparent);
}

BOOST_AUTO_TEST_CASE(replay_embeds_reference_once_and_does_not_modify_original_svg)
{
    showcase::ReplayAssets assets;
    const auto uri = bitmapUri(Image{1, 1, {{1, 2, 3}}});
    const auto original = "<svg><image href='" + uri + "'/></svg>";
    const auto packed = assets.pack(original);
    BOOST_CHECK_EQUAL(packed, "<svg><image data-asset='0'/></svg>");
    BOOST_CHECK_EQUAL(assets.pack(original), packed);
    BOOST_CHECK_EQUAL(assets.json(), "[\"" + uri + "\"]");
    BOOST_CHECK(original.find(uri) != std::string::npos);
}
