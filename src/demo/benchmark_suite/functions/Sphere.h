#pragma once

/**
 * The Sphere function has d (d is the dimension) local minima except for the global one.
 * It is continuous, convex and unimodal. The plot shows its two-dimensional form.
 */

#include "MultiVariableOptimProblem.h"
#include <vector>

class Sphere: public MultiVariableOptimiProblem
{
public:
    double compute(const std::vector<double>& coordinates) const override;

    std::vector<double> getMinima(const unsigned int num_dimension) const override;

    std::string getName() override
    {
        return "Sphere";
    }
};

