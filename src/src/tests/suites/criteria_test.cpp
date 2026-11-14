#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE criteria
#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif
#include <geneial/algorithm/criteria/CombinedCriterion.h>
#include <geneial/algorithm/criteria/ConsecutiveDecorator.h>
#include <geneial/algorithm/criteria/FitnessValueDeltaCriterion.h>
#include <geneial/algorithm/criteria/FitnessValueReachedCriterion.h>
#include <geneial/algorithm/criteria/FixPointCriterion.h>
#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>
#include <geneial/algorithm/criteria/NegationDecorator.h>
#include <geneial/algorithm/criteria/PopulationUnchangedCriterion.h>
#include <geneial/algorithm/criteria/TrailingStopCriterion.h>
#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>

#include <cstdint>
#include <limits>
#include <sstream>
#include <type_traits>

using namespace geneial::algorithm::stopping_criteria;
using Manager = geneial::population::management::BaseManager<double>;
using Factory = geneial::population::chromosome::MultiValueChromosomeFactory<int, double>;
struct Fixture
{
    std::shared_ptr<Manager> manager;
    Fixture()
    {
        auto evaluator =
            std::make_shared<geneial::FunctionFitnessEvaluator<int>>([](std::span<const int>) { return 10.0; });
        auto factory = Factory::Builder(evaluator).create();
        manager = Manager::create(factory);
        manager->getPopulation().insertChromosome(factory->createChromosome());
    }

    void replaceFitness(double value)
    {
        auto candidate = manager->getChromosomeFactory().createChromosome();
        candidate->setFitness(std::make_unique<geneial::Fitness<double>>(value));
        manager->getPopulation().clearChromosomes();
        manager->getPopulation().insertChromosome(candidate);
    }
};
class Constant final : public BaseStoppingCriterion<double>
{
    bool _value;

  public:
    explicit Constant(bool value) : _value(value) {}
    bool wasReached(Manager &) override
    {
        return _value;
    }
    void print(std::ostream &stream) const override
    {
        stream << _value;
    }
};

class CountingCriterion final : public BaseStoppingCriterion<double>
{
  public:
    bool value = true;
    unsigned int calls = 0;
    bool wasReached(Manager &) override
    {
        ++calls;
        return value;
    }
    void print(std::ostream &stream) const override
    {
        stream << "CountingCriterion";
    }
};
BOOST_FIXTURE_TEST_SUITE(Criteria, Fixture)
BOOST_AUTO_TEST_CASE(combinations_follow_the_boolean_truth_tables)
{
    using Combined = CombinedCriterion<double>;
    for (bool first : {false, true})
        for (bool second : {false, true})
            for (const auto glue : {Combined::AND, Combined::OR, Combined::XOR})
            {
                Combined criterion;
                criterion.add(Combined::INIT, std::make_shared<Constant>(first));
                criterion.add(glue, std::make_shared<Constant>(second));
                const bool expected = glue == Combined::AND  ? first && second
                                      : glue == Combined::OR ? first || second
                                                             : first != second;
                BOOST_CHECK_EQUAL(criterion.wasReached(*manager), expected);
                NegationDecorator<double> negated(std::make_shared<Constant>(first));
                BOOST_CHECK_EQUAL(negated.wasReached(*manager), !first);
            }
}
BOOST_AUTO_TEST_CASE(empty_and_invalid_combinations_have_defined_behavior)
{
    using Combined = CombinedCriterion<double>;
    Combined criterion;
    BOOST_CHECK(criterion.wasReached(*manager));
    criterion.add(Combined::AND, std::make_shared<Constant>(true));
    BOOST_CHECK_THROW(criterion.wasReached(*manager), std::invalid_argument);
    criterion.setCriteria({{Combined::INIT, nullptr}});
    BOOST_CHECK_THROW(criterion.wasReached(*manager), std::invalid_argument);
}
BOOST_AUTO_TEST_CASE(tolerance_is_symmetric_and_bounded)
{
    for (const double desired : {9.0, 10.0, 11.0})
    {
        FitnessValueDeltaCriterion<double> criterion(desired, 1);
        BOOST_CHECK(criterion.wasReached(*manager));
    }
    for (const double desired : {-100.0, 8.99, 11.01, 100.0})
    {
        FitnessValueDeltaCriterion<double> criterion(desired, 1);
        BOOST_CHECK(!criterion.wasReached(*manager));
    }
    BOOST_CHECK_THROW(FitnessValueDeltaCriterion<double>(10, -1), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(consecutive_decorator_counts_window_hits_once_per_generation)
{
    auto inner = std::make_shared<CountingCriterion>();
    ConsecutiveDecorator<double> criterion(3, 2, inner);
    BOOST_CHECK(!criterion.wasReached(*manager));
    BOOST_CHECK(!criterion.wasReached(*manager));
    BOOST_CHECK_EQUAL(inner->calls, 1);
    manager->getPopulation().doAge();
    inner->value = false;
    BOOST_CHECK(!criterion.wasReached(*manager));
    manager->getPopulation().doAge();
    inner->value = true;
    BOOST_CHECK(criterion.wasReached(*manager));
    BOOST_CHECK(criterion.wasReached(*manager));
    BOOST_CHECK_EQUAL(inner->calls, 3);
    inner->value = false;
    manager->getPopulation().setAge(5);
    BOOST_CHECK(!criterion.wasReached(*manager));
}

BOOST_AUTO_TEST_CASE(stateful_criteria_reset_across_managers_age_rewinds_and_manual_restarts)
{
    FixPointCriterion<double> criterion;
    BOOST_CHECK(!criterion.wasReached(*manager));
    manager->getPopulation().doAge();
    BOOST_CHECK(criterion.wasReached(*manager));
    Fixture another;
    BOOST_CHECK(!criterion.wasReached(*another.manager));
    another.manager->getPopulation().doAge();
    BOOST_CHECK(criterion.wasReached(*another.manager));
    another.manager->getPopulation().setAge(0);
    BOOST_CHECK(!criterion.wasReached(*another.manager));
    another.manager->getPopulation().doAge();
    BOOST_CHECK(criterion.wasReached(*another.manager));
    criterion.reset();
    BOOST_CHECK(!criterion.wasReached(*another.manager));
}

BOOST_AUTO_TEST_CASE(replacing_a_decorated_criterion_resets_its_history)
{
    auto first = std::make_shared<Constant>(true);
    auto second = std::make_shared<Constant>(true);
    ConsecutiveDecorator<double> criterion(3, 2, first);
    BOOST_CHECK(!criterion.wasReached(*manager));
    manager->getPopulation().doAge();
    BOOST_CHECK(criterion.wasReached(*manager));
    criterion.setCriterion(second);
    BOOST_CHECK(&criterion.getCriterion() == second.get());
    BOOST_CHECK(!criterion.wasReached(*manager));
    BOOST_CHECK_THROW(criterion.setCriterion(nullptr), std::invalid_argument);
    BOOST_CHECK(&criterion.getCriterion() == second.get());
}

BOOST_AUTO_TEST_CASE(fix_point_requires_a_full_minimum_window_and_handles_improvement)
{
    FixPointCriterion<double> criterion(0, 3, 3);
    for (unsigned int generation = 0; generation < 4; ++generation)
    {
        manager->getPopulation().setAge(generation);
        replaceFitness(generation == 0 ? 10 : 12);
        BOOST_CHECK_EQUAL(criterion.wasReached(*manager), generation == 3);
        BOOST_CHECK_EQUAL(criterion.wasReached(*manager), generation == 3);
    }
    manager->getPopulation().doAge();
    replaceFitness(9);
    BOOST_CHECK(!criterion.wasReached(*manager));
}

BOOST_AUTO_TEST_CASE(integer_criteria_compare_full_range_and_adjacent_scores_exactly)
{
    const auto check = []<typename Score>()
    {
        using IntegerFactory = geneial::population::chromosome::MultiValueChromosomeFactory<int, Score>;
        using IntegerManager = geneial::population::management::BaseManager<Score>;
        auto evaluator = std::make_shared<geneial::FunctionFitnessEvaluator<int, Score>>([](std::span<const int>)
                                                                                         { return Score(0); });
        auto integerManager = IntegerManager::create(typename IntegerFactory::Builder(evaluator).create());
        const auto observe = [&](Score score)
        {
            auto candidate = integerManager->getChromosomeFactory().createChromosome();
            candidate->setFitness(std::make_unique<geneial::Fitness<Score>>(score));
            integerManager->getPopulation().clearChromosomes();
            integerManager->getPopulation().insertChromosome(candidate);
            integerManager->getPopulation().doAge();
        };
        const auto lowest = std::numeric_limits<Score>::lowest();
        const auto highest = std::numeric_limits<Score>::max();
        FixPointCriterion<Score> fixPoint(0, 2, 2);
        TrailingStopCriterion<Score> trailing(1);
        for (const auto score : {lowest, highest})
        {
            observe(score);
            BOOST_CHECK(!fixPoint.wasReached(*integerManager));
            BOOST_CHECK(!trailing.wasReached(*integerManager));
        }
        observe(highest - 1);
        BOOST_CHECK(trailing.wasReached(*integerManager));
        BOOST_CHECK(!FitnessValueDeltaCriterion<Score>(highest, 0).wasReached(*integerManager));
        BOOST_CHECK(FitnessValueDeltaCriterion<Score>(highest, 1).wasReached(*integerManager));
        fixPoint.reset();
        BOOST_CHECK(!fixPoint.wasReached(*integerManager));
        observe(highest);
        BOOST_CHECK(!fixPoint.wasReached(*integerManager));
        observe(highest);
        BOOST_CHECK(fixPoint.wasReached(*integerManager));
        observe(lowest);
        BOOST_CHECK(!FitnessValueDeltaCriterion<Score>(highest, highest - 1).wasReached(*integerManager));
        BOOST_CHECK(trailing.wasReached(*integerManager));
        BOOST_CHECK(!fixPoint.wasReached(*integerManager));
    };
    check.template operator()<std::int64_t>();
    check.template operator()<std::uint64_t>();
}

BOOST_AUTO_TEST_CASE(population_unchanged_compares_genomes_not_age_extremes)
{
    using Chromosome = geneial::population::chromosome::MultiValueChromosome<int, double>;
    static_assert(std::is_convertible_v<PopulationUnchangedCriterion<double> *, BaseStoppingCriterion<double> *>);
    PopulationUnchangedCriterion<double> criterion;
    auto original = std::dynamic_pointer_cast<Chromosome>(manager->getHighestFitnessChromosome());
    manager->getPopulation().removeChromosome(original);
    original->setValueContainer({1});
    manager->getPopulation().insertChromosome(original);
    BOOST_CHECK(!criterion.wasReached(*manager));
    auto clone = std::make_shared<Chromosome>(*original);
    clone->setAge(100);
    manager->getPopulation().clearChromosomes();
    manager->getPopulation().insertChromosome(clone);
    manager->getPopulation().doAge();
    BOOST_CHECK(criterion.wasReached(*manager));
    manager->getPopulation().removeChromosome(clone);
    clone->setValueContainer({2});
    manager->getPopulation().insertChromosome(clone);
    manager->getPopulation().doAge();
    BOOST_CHECK(!criterion.wasReached(*manager));
    replaceFitness(10);
    manager->getPopulation().doAge();
    BOOST_CHECK(!criterion.wasReached(*manager));
    manager->getPopulation().clearChromosomes();
    manager->getPopulation().doAge();
    BOOST_CHECK(!criterion.wasReached(*manager));
    manager->getPopulation().doAge();
    BOOST_CHECK(criterion.wasReached(*manager));
}

BOOST_AUTO_TEST_CASE(trailing_stop_tracks_the_best_observed_fitness_and_exact_drawdown)
{
    TrailingStopCriterion<double> criterion(3);
    const std::vector<double> scores{10, 12, 11, 15, 12};
    for (unsigned int generation = 0; generation < scores.size(); ++generation)
    {
        manager->getPopulation().setAge(generation);
        replaceFitness(scores[generation]);
        BOOST_CHECK_EQUAL(criterion.wasReached(*manager), generation == 4);
        BOOST_CHECK_EQUAL(criterion.wasReached(*manager), generation == 4);
    }
    criterion.reset();
    BOOST_CHECK(!criterion.wasReached(*manager));
    TrailingStopCriterion<double> anyDecline;
    BOOST_CHECK(!anyDecline.wasReached(*manager));
    manager->getPopulation().doAge();
    BOOST_CHECK(!anyDecline.wasReached(*manager));
    replaceFitness(11.99);
    manager->getPopulation().doAge();
    BOOST_CHECK(anyDecline.wasReached(*manager));
}

BOOST_AUTO_TEST_CASE(generation_limits_have_checked_boundaries)
{
    MaxGenerationCriterion<double> zero(0), one(1);
    BOOST_CHECK(zero.wasReached(*manager));
    BOOST_CHECK(!one.wasReached(*manager));
    manager->getPopulation().doAge();
    BOOST_CHECK(one.wasReached(*manager));
    BOOST_CHECK_THROW(MaxGenerationCriterion<double>(-1), std::invalid_argument);
    if constexpr (std::numeric_limits<long>::max() > std::numeric_limits<unsigned int>::max())
        BOOST_CHECK_THROW((MaxGenerationCriterion<double>(std::numeric_limits<long>::max())), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(invalid_stopping_configuration_throws_instead_of_asserting_or_silently_never_stopping)
{
    auto inner = std::make_shared<Constant>(true);
    BOOST_CHECK_THROW(ConsecutiveDecorator<double>(0, 0, inner), std::invalid_argument);
    BOOST_CHECK_THROW(ConsecutiveDecorator<double>(2, 3, inner), std::invalid_argument);
    BOOST_CHECK_THROW(ConsecutiveDecorator<double>(2, 1, nullptr), std::invalid_argument);
    BOOST_CHECK_THROW(NegationDecorator<double>(nullptr), std::invalid_argument);
    NegationDecorator<double> negated(inner);
    BOOST_CHECK_THROW(negated.setCriterion(nullptr), std::invalid_argument);
    BOOST_CHECK(!negated.wasReached(*manager));
    std::ostringstream printed;
    negated.print(printed);
    BOOST_CHECK_EQUAL(printed.str(), "Negated (1)");
    BOOST_CHECK_THROW(FixPointCriterion<double>(-1, 3, 2), std::invalid_argument);
    BOOST_CHECK_THROW(FixPointCriterion<double>(0, 0, 0), std::invalid_argument);
    BOOST_CHECK_THROW(FixPointCriterion<double>(0, 3, 1), std::invalid_argument);
    BOOST_CHECK_THROW(FixPointCriterion<double>(0, 3, 4), std::invalid_argument);
    BOOST_CHECK_THROW(FixPointCriterion<double>(0, 3.5, 2), std::invalid_argument);
    BOOST_CHECK_THROW(FixPointCriterion<double>(0, -3, 2), std::invalid_argument);
    BOOST_CHECK_THROW(TrailingStopCriterion<double>(-1), std::invalid_argument);
    for (const double invalid : {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::infinity()})
    {
        BOOST_CHECK_THROW((FixPointCriterion<double>(invalid)), std::invalid_argument);
        BOOST_CHECK_THROW(FixPointCriterion<double>(0, invalid, 2), std::invalid_argument);
        BOOST_CHECK_THROW(FitnessValueDeltaCriterion<double>(invalid, 1), std::invalid_argument);
        BOOST_CHECK_THROW(FitnessValueDeltaCriterion<double>(10, invalid), std::invalid_argument);
        BOOST_CHECK_THROW((FitnessValueReachedCriterion<double>(invalid)), std::invalid_argument);
        BOOST_CHECK_THROW((TrailingStopCriterion<double>(invalid)), std::invalid_argument);
    }
}
BOOST_AUTO_TEST_SUITE_END()
