#pragma once

/**
 * The Ackley function is widely used for testing optimization algorithms.
 *
 * In its two-dimensional form, as shown in the plot above, it is characterized by a nearly flat outer region, and a large hole at the centre.
 * The function poses a risk for optimization algorithms, particularly hillclimbing algorithms, to be trapped in one of its many local minima.
 */
#include "MultiVariableOptimProblem.h"
#include <vector>

class Ackley : public MultiVariableOptimiProblem
{
public:
    double compute(const std::vector<double>& coordinates) const override;
    std::vector<double> getMinima(const unsigned int num_dimension) const  override;
    std::string getName() override
    {
        return "Ackley";
    }
};
