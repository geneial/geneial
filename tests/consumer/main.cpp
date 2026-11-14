#include <cmath>
#include <geneial/algorithm/criteria/CombinedCriterion.h>
#include <geneial/algorithm/criteria/PopulationUnchangedCriterion.h>
#include <geneial/algorithm/diagnostics/Diagnostics.h>
#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/utility/Random.h>
#include <numeric>
#include <sstream>

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
    auto criterion = std::make_shared<geneial::algorithm::stopping_criteria::PopulationUnchangedCriterion<double>>();
    using Combined = geneial::algorithm::stopping_criteria::CombinedCriterion<double>;
    Combined root({{Combined::INIT, criterion}});
    if (root.wasReached(*manager))
        return 2;
    population.doAge();
    if (!root.wasReached(*manager))
        return 3;
    root.reset();
    if (root.wasReached(*manager))
        return 4;
    using Factory = geneial::population::chromosome::MultiValueChromosomeFactory<double, double>;
    auto recycling = std::dynamic_pointer_cast<Factory>(factory);
    recycling->setResourcePoolCapacity(0);
    factory->createChromosome().reset();
    if (recycling->getCachedChromosomeCount() != 0)
        return 5;
    geneial::population::management::StatisticBookkeeper keeper(1);
    geneial::population::management::EventValueData<double>::create(keeper, "timing", 1);
    geneial::population::management::EventValueData<double>::create(keeper, "timing", 2);
    std::ostringstream timing;
    geneial::algorithm::printTiming("timing", keeper.getEvents(), timing);
    return keeper.getDroppedEventCount() == 1 && timing.str().find("1.000000") != std::string::npos ? 0 : 6;
}
