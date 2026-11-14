#include <cmath>
#include <geneial/algorithm/criteria/PopulationUnchangedCriterion.h>
#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/core/population/management/BaseManager.h>
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
    const auto factory = builder.create();
    const auto candidate = factory->createChromosome();
    auto manager = geneial::population::management::BaseManager<double>::create(factory);
    auto &population = manager->getPopulation();
    if (!std::isfinite(candidate->getFitness().get()) || !population.insertChromosome(candidate) ||
        !population.containsChromosome(candidate) || population.getChromosomeByHash(candidate->getHash()) != candidate)
        return 1;
    geneial::algorithm::stopping_criteria::PopulationUnchangedCriterion<double> criterion;
    if (criterion.wasReached(*manager))
        return 2;
    population.doAge();
    return criterion.wasReached(*manager) ? 0 : 3;
}
