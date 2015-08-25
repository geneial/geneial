#pragma once

/**
 * Base class for Multivariable optimization problems.
 */
#include <vector>
#include <string>

class MultiVariableOptimiProblem
{
public:
    virtual double compute(const std::vector<double>& coordinates) const = 0;
    virtual std::vector<double> getMinima(const unsigned int num_dimension) const = 0;
    virtual std::string getName() = 0;
};

