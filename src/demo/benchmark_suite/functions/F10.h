#pragma once

/**
 * This is F10 from Practical Genetic Algorithms, Haupt, Wiley Second Edition, p. 205
 */

#include "MultiVariableOptimProblem.h"
#include <vector>

class F10: public MultiVariableOptimiProblem
{
public:
    double compute(const std::vector<double>& coordinates) const override;
    std::vector<double> getMinima(const unsigned int num_dimension) const override;
    std::string getName() override
    {
        return "F10";
    }
};

