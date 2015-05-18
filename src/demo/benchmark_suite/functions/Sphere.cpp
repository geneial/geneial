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
