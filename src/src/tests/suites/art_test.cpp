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
