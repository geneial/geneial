#pragma once

/**
 * Base class for Multivariable optimization problems.
 */
#include <vector>

class MultiVariableOptimiProblem
{
public:
    virtual double compute(const std::vector<double>& coordinates) const = 0;
};

