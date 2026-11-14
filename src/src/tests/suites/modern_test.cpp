#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE modern
#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif
#include <atomic>
#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/core/operations/crossover/MultiValueCrossoverLambdaAdapter.h>
#include <geneial/core/operations/mutation/UniformMutationOperation.h>
#include <geneial/core/operations/mutation/ValueSwapMutationOperation.h>
#include <geneial/core/operations/replacement/ReplaceWorstOperation.h>
#include <geneial/core/operations/selection/FitnessProportionalSelection.h>
#include <geneial/core/operations/selection/UniformRandomSelection.h>
#include <geneial/utility/ThreadedExecutionManager.h>
#include <numeric>

using Chromosome = geneial::population::chromosome::MultiValueChromosome<int, double>;
using Factory = geneial::population::chromosome::MultiValueChromosomeFactory<int, double>;
using Manager = geneial::population::management::BaseManager<double>;
using geneial::utility::Random;

struct Fixture
{
    std::shared_ptr<Factory> factory;
    Fixture()
    {
        Random::initialize(42);
        auto evaluator = std::make_shared<geneial::FunctionFitnessEvaluator<int>>(
            [](std::span<const int> values) { return std::accumulate(values.begin(), values.end(), 0.0); });
        Factory::Builder builder(evaluator);
        builder.getSettings().setNum(12);
        factory = std::dynamic_pointer_cast<Factory>(builder.create());
    }
    Chromosome::ptr chromosome(int value)
    {
        auto result = std::dynamic_pointer_cast<Chromosome>(factory->createChromosome());
        result->setValueContainer(std::vector<int>(12, value));
        return result;
    }
};



BOOST_FIXTURE_TEST_SUITE(Modern, Fixture)

BOOST_AUTO_TEST_CASE(chromosome_updates_invalidate_fitness_and_swap_caches)
{
    auto first = chromosome(-2), second = chromosome(5);
    BOOST_CHECK_EQUAL(first->getAverage(), -2);
    BOOST_CHECK_EQUAL(first->getFitness().get(), -24);
    first->getContainer()[0] = 10;
    BOOST_CHECK_EQUAL(first->getFitness().get(), -12);
    const auto oldHash = first->getHash();
    BOOST_CHECK_EQUAL(second->getFitness().get(), 60);
    first->swap(*second);
    BOOST_CHECK_EQUAL(second->getHash(), oldHash);
    BOOST_CHECK_EQUAL(first->getFitness().get(), 60);
    BOOST_CHECK_EQUAL(second->getFitness().get(), -12);
    BOOST_CHECK(!first->equals(*second));
    using Evaluator = geneial::FunctionFitnessEvaluator<int>;
    BOOST_CHECK_THROW(Evaluator({}), std::invalid_argument);
    geneial::Fitness<double> empty;
    BOOST_CHECK_THROW(empty.get(), std::bad_optional_access);
}

BOOST_AUTO_TEST_CASE(n_point_crossover_preserves_nonempty_minimum_width_segments)
{
    using namespace geneial::operation::crossover;
    auto first = chromosome(1), second = chromosome(2);
    MultiValueChromosomeNPointCrossover<int, double>::Builder builder(factory);
    builder.getCrossoverSettings().setCrossOverPoints(3);
    builder.getCrossoverSettings().setWidthSetting(MultiValueChromosomeNPointCrossoverSettings::RANDOM_MIN_WIDTH);
    builder.getCrossoverSettings().setMinWidth(3);
    const auto operation = builder.create();
    for (unsigned int trial = 0; trial < 100; ++trial)
    {
        const auto child = std::dynamic_pointer_cast<Chromosome>(operation->doCrossover(first, second).front());
        const auto &genes = std::as_const(*child).getContainer();
        for (unsigned int index = 0; index < 12; ++index)
            BOOST_CHECK_EQUAL(genes[index], index / 3 % 2 + 1);
    }
    builder.getCrossoverSettings().setMinWidth(4);
    BOOST_CHECK_THROW(operation->doCrossover(first, second), std::invalid_argument);
    builder.getCrossoverSettings().setWidthSetting(MultiValueChromosomeNPointCrossoverSettings::EQUIDISTANT_WIDTH);
    BOOST_CHECK_EQUAL(
        std::dynamic_pointer_cast<Chromosome>(operation->doCrossover(first, second).front())->getContainer()[3], 2);
    builder.getCrossoverSettings().setCrossOverPoints(12);
    BOOST_CHECK_THROW(operation->doCrossover(first, second), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(lambda_crossover_compiles_runs_and_reports_symmetry)
{
    using Adapter = geneial::operation::crossover::MultiValueCrossoverLambdaAdapter<int, double>;
    Adapter::Builder builder(
        factory,
        [this](Adapter::arg_type first, Adapter::arg_type second, const Adapter &adapter) -> Adapter::return_type
        { return {chromosome(adapter.isSymmetric() ? 7 : 0)}; }, true);
    const auto operation = builder.create();
    BOOST_CHECK(operation->isSymmetric());
    BOOST_CHECK_EQUAL(operation->doCrossover(chromosome(1), chromosome(2)).front()->getFitness().get(), 84);
    Adapter::Builder invalid(factory, {});
    BOOST_CHECK_THROW(invalid.create(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(impossible_mutation_and_population_requests_fail_instead_of_hanging)
{
    auto manager = Manager::create(factory);
    BOOST_CHECK_THROW(manager->getHighestFitness(), std::logic_error);
    BOOST_CHECK_THROW(manager->getLowestFitnessChromosome(), std::logic_error);
    auto chooser = geneial::operation::choosing::ChooseRandom<double>::Builder(1).create();
    geneial::operation::mutation::UniformMutationOperation<int, double>::Builder uniform(factory);
    uniform.setChoosingOperation(chooser);
    uniform.getSettings().setMaximumPointsToMutate(13);
    BOOST_CHECK_THROW(uniform.create()->doMutate({chromosome(1)}, *manager), std::invalid_argument);
    geneial::operation::mutation::ValueSwapMutationOperation<int, double>::Builder swaps(factory);
    swaps.setChoosingOperation(chooser);
    swaps.getSettings().setMaximumPointsToMutate(200);
    BOOST_CHECK_THROW(swaps.create()->doMutate({chromosome(1)}, *manager), std::invalid_argument);
    auto settings = factory->getSettings();
    settings.setRandomMin(0);
    settings.setRandomMax(0);
    factory->setSettings(settings);
    manager->getPopulationSettings().setMaxChromosomes(2);
    manager->getPopulationSettings().setMaxReplenishAttempts(5);
    BOOST_CHECK_THROW(manager->replenishPopulation(), std::runtime_error);
}
BOOST_AUTO_TEST_CASE(settings_replacement_and_small_populations_have_defined_behavior)
{
    auto manager = Manager::create(factory);
    manager->getPopulation().insertChromosome(chromosome(1));
    manager->getPopulation().insertChromosome(chromosome(2));
    using namespace geneial::operation::selection;
    auto selection = FitnessProportionalSelection<double>::Builder().create();
    selection->setSettings(std::make_shared<FitnessProportionalSelectionSettings>(2, 2));
    BOOST_CHECK_EQUAL(selection->doSelect(manager->getPopulation(), *manager).size(), 2);
    BOOST_CHECK_THROW(selection->setSettings(std::make_shared<BaseSelectionSettings>(1)), std::invalid_argument);
    UniformRandomSelection<double>::Builder uniform;
    uniform.getSettings().setNumberOfParents(2);
    const auto chosen = uniform.create()->doSelect(manager->getPopulation(), *manager);
    BOOST_REQUIRE_EQUAL(chosen.size(), 2);
    BOOST_CHECK(chosen[0] != chosen[1]);
    uniform.getSettings().setNumberOfParents(3);
    BOOST_CHECK_THROW(uniform.create()->doSelect(manager->getPopulation(), *manager), std::invalid_argument);
    using namespace geneial::operation::replacement;
    const auto replacement = ReplaceWorstOperation<double>::Builder().create();
    BaseReplacementSettings settings;
    settings.setAmountElitism(100);
    replacement->setSettings(settings);
    geneial::population::Population<double>::chromosome_container children{chromosome(3)};
    replacement->doReplace(manager->getPopulation(), {}, children, *manager);
    BOOST_CHECK_EQUAL(manager->getPopulation().getSize(), 2);
    settings.setAmountElitism(1);
    replacement->setSettings(settings);
    replacement->doReplace(manager->getPopulation(), {}, children, *manager);
    BOOST_CHECK_EQUAL(manager->getPopulation().getSize(), 3);
}

BOOST_AUTO_TEST_SUITE_END()
