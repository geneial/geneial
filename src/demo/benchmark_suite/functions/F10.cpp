#include "F10.h"

#include <numeric>
#include <cmath>
#include <iostream>

double F10::compute(const std::vector<double>& coordinates) const
{
    constexpr const double C_PI = 3.14159265358979323846;
    const double term = 10*coordinates.size();


    const double sum = term + std::accumulate(coordinates.begin(), coordinates.end(), 0.0, [](double a, double b)
    {
        double innerterm = std::pow(b,2) - 10.0 * std::cos(2*C_PI*b);
        return a + innerterm;
    });
    return sum;
}

std::vector<double> F10::getMinima(const unsigned int num_dimension) const
{
    unsigned int dimleft = num_dimension;
    std::vector<double> res;
    while(dimleft--)
    {
        res.emplace_back(0);
    }
    return res;
}
