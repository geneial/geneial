#include "F5.h"

#include <numeric>
#include <cmath>
#include <iostream>

double F5::compute(const std::vector<double>& coordinates) const
{
    constexpr const double C_PI = 3.14159265358979323846;
    const double sum = std::accumulate(coordinates.begin(), coordinates.end(), 0.0, [](double a, double b)
    {
        double innerterm =  std::abs(b) -  std::cos(std::sqrt(std::abs(10*b)));
        return a + innerterm;
    });
    return sum;
}

std::vector<double> F5::getMinima(const unsigned int num_dimension) const
{
    unsigned int dimleft = num_dimension;
    std::vector<double> res;
    while(dimleft--)
    {
        res.emplace_back(0);
    }
    return res;
}
