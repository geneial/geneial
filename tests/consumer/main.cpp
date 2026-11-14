#include <cmath>
#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/utility/Random.h>
#include <numeric>

int main()
{
    geneial::utility::Random::initialize(42);
    auto evaluator = std::make_shared<geneial::FunctionFitnessEvaluator<double>>(
        [](std::span<const double> values)
        { return -std::inner_product(values.begin(), values.end(), values.begin(), 0.0); });
    geneial::population::chromosome::MultiValueChromosomeFactory<double, double>::Builder builder(evaluator);
    builder.getSettings().setNum(8);
    const auto candidate = builder.create()->createChromosome();
    return std::isfinite(candidate->getFitness().get()) ? 0 : 1;
}
