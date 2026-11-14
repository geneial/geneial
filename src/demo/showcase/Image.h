#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <fstream>
#include <limits>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace showcase::art
{
using Pixel = std::array<int, 3>;

struct Image
{
    unsigned int width;
    unsigned int height;
    std::vector<Pixel> pixels;

    const Pixel &at(unsigned int column, unsigned int row) const
    {
        return pixels.at(row * width + column);
    }
};

inline std::string token(std::istream &stream)
{
    std::string result;
    for (;;)
    {
        stream >> std::ws;
        if (stream.peek() != '#')
            break;
        stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    stream >> result;
    if (!stream)
        throw std::runtime_error("Truncated PPM image");
    return result;
}

inline unsigned int imageNumber(std::istream &stream, unsigned int minimum, unsigned int maximum)
{
    const auto text = token(stream);
    if (text.find_first_not_of("0123456789") != std::string::npos)
        throw std::runtime_error("Invalid number in PPM image");
    const auto number = std::stoull(text);
    if (number < minimum || number > maximum)
        throw std::runtime_error("PPM value outside supported range");
    return static_cast<unsigned int>(number);
}

inline Image readPpm(std::istream &stream)
{
    const auto format = token(stream);
    if (format != "P3" && format != "P6")
        throw std::runtime_error("Expected P3 or P6 PPM image");
    Image image{imageNumber(stream, 1, 4096), imageNumber(stream, 1, 4096), {}};
    const auto maximum = imageNumber(stream, 1, 255);
    image.pixels.resize(static_cast<std::size_t>(image.width) * image.height);
    if (format == "P6")
    {
        const int separator = stream.get();
        if (separator == std::char_traits<char>::eof() || !std::isspace(static_cast<unsigned char>(separator)))
            throw std::runtime_error("Missing binary PPM separator");
        if (separator == '\r' && stream.peek() == '\n')
            stream.get();
    }
    for (auto &pixel : image.pixels)
    {
        for (auto &channel : pixel)
        {
            const int value = format == "P3" ? static_cast<int>(imageNumber(stream, 0, maximum)) : stream.get();
            if (value < 0 || value > static_cast<int>(maximum))
                throw std::runtime_error("Invalid or truncated PPM raster");
            channel = value * 255 / maximum;
        }
    }
    return image;
}

inline Image resizeImage(const Image &original, unsigned int maximumEdge)
{
    if (maximumEdge == 0 || original.width == 0 || original.height == 0 ||
        original.pixels.size() != static_cast<std::size_t>(original.width) * original.height)
        throw std::invalid_argument("Cannot resize an empty or malformed image");
    const double scale = std::min(1.0, static_cast<double>(maximumEdge) / std::max(original.width, original.height));
    if (scale == 1)
        return original;
    Image result{std::max(1u, static_cast<unsigned int>(std::lround(original.width * scale))),
                 std::max(1u, static_cast<unsigned int>(std::lround(original.height * scale))),
                 {}};
    result.pixels.reserve(static_cast<std::size_t>(result.width) * result.height);
    for (unsigned int row = 0; row < result.height; ++row)
        for (unsigned int column = 0; column < result.width; ++column)
        {
            const double left = static_cast<double>(column) * original.width / result.width;
            const double right = static_cast<double>(column + 1) * original.width / result.width;
            const double top = static_cast<double>(row) * original.height / result.height;
            const double bottom = static_cast<double>(row + 1) * original.height / result.height;
            std::array<double, 3> sum{};
            for (unsigned int sourceRow = static_cast<unsigned int>(top);
                 sourceRow < std::min<double>(original.height, std::ceil(bottom)); ++sourceRow)
                for (unsigned int sourceColumn = static_cast<unsigned int>(left);
                     sourceColumn < std::min<double>(original.width, std::ceil(right)); ++sourceColumn)
                {
                    const double weight =
                        (std::min(right, sourceColumn + 1.0) - std::max(left, static_cast<double>(sourceColumn))) *
                        (std::min(bottom, sourceRow + 1.0) - std::max(top, static_cast<double>(sourceRow)));
                    for (unsigned int channel = 0; channel < 3; ++channel)
                        sum[channel] += weight * original.at(sourceColumn, sourceRow)[channel];
                }
            Pixel pixel{};
            for (unsigned int channel = 0; channel < 3; ++channel)
                pixel[channel] = static_cast<int>(std::lround(sum[channel] / ((right - left) * (bottom - top))));
            result.pixels.push_back(pixel);
        }
    return result;
}

inline Image loadTarget(const std::string &path, unsigned int maximumEdge = 600)
{
    std::ifstream stream(path, std::ios::binary);
    if (!stream)
        throw std::runtime_error("Cannot open target image: " + path);
    return resizeImage(readPpm(stream), maximumEdge);
}

inline std::string ppm(const Image &image)
{
    std::ostringstream result;
    result << "P3\n" << image.width << ' ' << image.height << "\n255\n";
    for (const auto &pixel : image.pixels)
        result << pixel[0] << ' ' << pixel[1] << ' ' << pixel[2] << '\n';
    return result.str();
}

inline double meanSquaredError(const Image &actual, const Image &target)
{
    if (actual.width != target.width || actual.height != target.height ||
        actual.pixels.size() != target.pixels.size() || actual.pixels.empty())
        throw std::invalid_argument("Images must have matching nonempty dimensions");
    double error = 0;
    for (std::size_t index = 0; index < actual.pixels.size(); ++index)
        for (unsigned int channel = 0; channel < 3; ++channel)
        {
            const double delta = actual.pixels[index][channel] - target.pixels[index][channel];
            error += delta * delta;
        }
    return error / (3 * actual.pixels.size());
}

inline std::string bitmapUri(const Image &image)
{
    const unsigned int stride = (image.width * 3 + 3) & ~3u;
    const unsigned int size = 54 + stride * image.height;
    std::vector<unsigned char> bytes(size, 0);
    auto put = [&bytes](unsigned int offset, unsigned int value, unsigned int count = 4)
    {
        for (unsigned int byte = 0; byte < count; ++byte)
            bytes[offset + byte] = (value >> (byte * 8)) & 255;
    };
    bytes[0] = 'B';
    bytes[1] = 'M';
    put(2, size);
    put(10, 54);
    put(14, 40);
    put(18, image.width);
    put(22, image.height);
    put(26, 1, 2);
    put(28, 24, 2);
    for (unsigned int row = 0; row < image.height; ++row)
        for (unsigned int column = 0; column < image.width; ++column)
            for (unsigned int channel = 0; channel < 3; ++channel)
                bytes[54 + (image.height - row - 1) * stride + column * 3 + channel] =
                    image.at(column, row)[2 - channel];
    constexpr char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string encoded = "data:image/bmp;base64,";
    for (std::size_t offset = 0; offset < bytes.size(); offset += 3)
    {
        const unsigned int value = (bytes[offset] << 16) | (offset + 1 < bytes.size() ? bytes[offset + 1] << 8 : 0) |
                                   (offset + 2 < bytes.size() ? bytes[offset + 2] : 0);
        encoded += alphabet[(value >> 18) & 63];
        encoded += alphabet[(value >> 12) & 63];
        encoded += offset + 1 < bytes.size() ? alphabet[(value >> 6) & 63] : '=';
        encoded += offset + 2 < bytes.size() ? alphabet[value & 63] : '=';
    }
    return encoded;
}
} // namespace showcase::art
