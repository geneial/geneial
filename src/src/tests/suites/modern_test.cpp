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


BOOST_AUTO_TEST_SUITE_END()
