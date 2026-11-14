#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE coupling
#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif
#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/operations/coupling/LambdaCouplingAdapter.h>
#include <geneial/core/operations/coupling/RandomCouplingOperation.h>
#include <geneial/core/operations/coupling/SimpleCouplingOperation.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeAverageCrossover.h>
#include <geneial/core/operations/crossover/MultiValueCrossoverLambdaAdapter.h>

BOOST_AUTO_TEST_CASE(coupling_respects_exact_offspring_count_and_rejects_empty_parent_pools)
{
    using Factory = geneial::population::chromosome::MultiValueChromosomeFactory<int, double>;
    auto evaluator = std::make_shared<geneial::FunctionFitnessEvaluator<int>>([](std::span<const int>) { return 0.0; });
    auto factory = std::dynamic_pointer_cast<Factory>(Factory::Builder(evaluator).create());
    auto manager = geneial::population::management::BaseManager<double>::create(factory);
    auto crossover =
        geneial::operation::crossover::MultiValueChromosomeAverageCrossover<int, double>::Builder(factory).create();
    geneial::operation::coupling::RandomCouplingOperation<double>::Builder builder;
    builder.getSettings().setNumberOfOffspring(7);
    auto operation = builder.create();
    auto first = factory->createChromosome(), second = factory->createChromosome();
    BOOST_CHECK_EQUAL(operation->doCopulate({first, second}, *crossover, *manager).size(), 7);
    BOOST_CHECK_THROW(operation->doCopulate({}, *crossover, *manager), std::invalid_argument);
    BOOST_CHECK_THROW(operation->doCopulate({first}, *crossover, *manager), std::invalid_argument);
    using Adapter = geneial::operation::coupling::LambdaCouplingAdapter<double>;
    auto adapter =
        Adapter::Builder([](const auto &parents, const auto &crossing, auto &managing) -> Adapter::offspring_result_set
                         { return crossing.doCrossover(parents[0], parents[1]); })
            .create();
    BOOST_CHECK_EQUAL(adapter->doCopulate({first, second}, *crossover, *manager).size(), 1);

    geneial::operation::coupling::SimpleCouplingOperation<double>::Builder simpleBuilder;
    simpleBuilder.getSettings().setNumberOfOffspring(7);
    auto simple = simpleBuilder.create();
    BOOST_CHECK_EQUAL(simple->doCopulate({first, second}, *crossover, *manager).size(), 7);
    BOOST_CHECK_THROW(simple->doCopulate({}, *crossover, *manager), std::invalid_argument);
    BOOST_CHECK_THROW(simple->doCopulate({first}, *crossover, *manager), std::invalid_argument);

    using EmptyCrossover = geneial::operation::crossover::MultiValueCrossoverLambdaAdapter<int, double>;
    auto empty = EmptyCrossover::Builder(factory, [](const auto &, const auto &, const auto &)
                                         { return EmptyCrossover::crossover_result_set{}; })
                     .create();
    BOOST_CHECK_THROW(simple->doCopulate({first, second}, *empty, *manager), std::runtime_error);
    BOOST_CHECK_THROW(operation->doCopulate({first, second}, *empty, *manager), std::runtime_error);
    simpleBuilder.getSettings().setNumberOfOffspring(0);
    BOOST_CHECK(simple->doCopulate({}, *empty, *manager).empty());
}
