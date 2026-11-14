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
#include <geneial/algorithm/criteria/FitnessValueDeltaCriterion.h>
#include <geneial/algorithm/criteria/NegationDecorator.h>
#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>

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
BOOST_AUTO_TEST_SUITE_END()
