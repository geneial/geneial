#include "Sphere.h"

#include <numeric>
#include <cmath>
#include <iostream>

double Sphere::compute(const std::vector<double>& coordinates) const
{
    const double sum = std::accumulate(coordinates.begin(), coordinates.end(), 0.0, [](double a, double b)
    {
        return a + std::pow(b,2);
    });
    return sum;
}

std::vector<double> Sphere::getMinima(const unsigned int num_dimension) const
{
    unsigned int dimleft = num_dimension;
    std::vector<double> res;
    while(dimleft--)
    {
        res.emplace_back(0);
    }
    return res;
}
