#include "Sphere.h"

#include <numeric>
#include <cmath>
#include <iostream>

namespace Sphere
{
double compute(const std::vector<double>& coordinates)
{
    const double sum = std::accumulate(coordinates.begin(), coordinates.end(), 0.0, [](double a, double b)
    {
        return a + std::pow(b,2);
    });
    return sum;
}

}
