#include "Ackley.h"

#include <vector>
#include <numeric>
#include <cmath>

double Ackley::compute(const std::vector<double>& coordinates) const
{
    constexpr const double C_PI = 3.14159265358979323846;
    constexpr const double a = 20;
    constexpr const double b = 0.2;
    constexpr const double c = 2 * C_PI;
    const double d = static_cast<double>(coordinates.size());

    const double squareparts = std::accumulate(coordinates.begin(), coordinates.end(), 0.0, [](double a, double b)
    {
        return a + std::pow(b,2);
    });

    const double cosparts = std::accumulate(coordinates.begin(), coordinates.end(), 0.0, [c](double a, double b)
    {
        return a + cos(c*b);
    });

    const double value = -a * exp(-b * sqrt((1.0 / d) * squareparts)) - exp((1.0 / d) * cosparts) + a + exp(1);

    return value;
}


std::vector<double> Ackley::getMinima(const unsigned int num_dimension) const
{
    unsigned int dimleft = num_dimension;
    std::vector<double> res;
    while(dimleft--)
    {
        res.emplace_back(0);
    }
    return res;
}
