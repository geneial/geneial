#define BOOST_TEST_DYN_LINK

/**
 * @ingroup unittests
 *
 * @brief Initialization is used to ensure all the geneial code compiles,
 * regardless of whether it is being used in some demo program or not.
 *
 * This test ensures the whole library is kept at a compiling state, even
 * if there are functions that are not currently being utilized by some test
 * program, benchmark or demo. Those functions might be used by some client which
 * then will discover the compile time error.
 *
 * This test is only compiled in the debug mode.
 *
 * This test can be compiled "exhaustively" to ensure max. compatibility
 * with int, double, float instantiations of all templates.
 *
 */

#define BOOST_TEST_MODULE compilation/template_instanciaton

#include <boost/test/unit_test.hpp>

#ifndef NDEBUG

#include <geneial/algorithm/SteadyStateAlgorithm.h>

#include <geneial/algorithm/criteria/CombinedCriterion.h>
#include <geneial/algorithm/criteria/ConsecutiveDecorator.h>
#include <geneial/algorithm/criteria/FitnessValueDeltaCriterion.h>
#include <geneial/algorithm/criteria/FitnessValueReachedCriterion.h>
#include <geneial/algorithm/criteria/FixPointCriterion.h>
#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>
#include <geneial/algorithm/criteria/NegationDecorator.h>
#include <geneial/algorithm/criteria/PopulationUnchangedCriterion.h>
#include <geneial/algorithm/criteria/StatefulStoppingCriterion.h>

#include <geneial/core/operations/choosing/ChooseRandom.h>
#include <geneial/core/operations/choosing/LambdaChoosingAdapter.h>

#include <geneial/core/operations/mutation/SmoothPeakMutationOperation.h>
#include <geneial/core/operations/mutation/UniformMutationOperation.h>
#include <geneial/core/operations/mutation/ValueSwapMutationOperation.h>

#include <geneial/core/operations/coupling/SimpleCouplingOperation.h>
#include <geneial/core/operations/coupling/RandomCouplingOperation.h>
#include <geneial/core/operations/coupling/LambdaCouplingAdapter.h>

#include <geneial/core/operations/crossover/MultiValueChromosomeAverageCrossover.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeBlendingCrossover.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/core/operations/crossover/SmoothedMultiValueChromosomeNPointCrossover.h>
#include <geneial/core/operations/crossover/MultiValuePermutationCrossover.h>



#include <geneial/core/operations/replacement/ReplaceRandomOperation.h>
#include <geneial/core/operations/replacement/ReplaceWorstOperation.h>

#include <geneial/core/operations/selection/FitnessProportionalSelection.h>
#include <geneial/core/operations/selection/RouletteWheelSelection.h>
#include <geneial/core/operations/selection/UniformRandomSelection.h>

#include <geneial/algorithm/diagnostics/Diagnostics.h>
#include <geneial/core/population/management/Bookkeeper.h>

#ifndef EXHAUSTIVE_INSTANCIATION_TEST

#define Expander1D(DECLTYPE) \
    template class DECLTYPE<double>;
#define Expander2D(DECLTYPE) \
    template class DECLTYPE<int,double>;

#else

#define Expander1D(DECLTYPE) \
    template class DECLTYPE<int>; \
    template class DECLTYPE<float>; \
    template class DECLTYPE<double>; \

#define Expander2D(DECLTYPE) \
    template class DECLTYPE<int,int>; \
    template class DECLTYPE<float,int>; \
    template class DECLTYPE<double,int>; \
    template class DECLTYPE<int,float>; \
    template class DECLTYPE<float,float>; \
    template class DECLTYPE<double,float>; \
    template class DECLTYPE<int,double>; \
    template class DECLTYPE<float,double>; \
    template class DECLTYPE<double,double>; \

#endif

Expander1D(geneial::algorithm::stopping_criteria::CombinedCriterion)
Expander1D(geneial::algorithm::stopping_criteria::ConsecutiveDecorator)
Expander1D(geneial::algorithm::stopping_criteria::FitnessValueDeltaCriterion)
Expander1D(geneial::algorithm::stopping_criteria::FitnessValueReachedCriterion)
Expander1D(geneial::algorithm::stopping_criteria::FixPointCriterion)
Expander1D(geneial::algorithm::stopping_criteria::MaxGenerationCriterion)
Expander1D(geneial::algorithm::stopping_criteria::NegationDecorator)
Expander1D(geneial::algorithm::stopping_criteria::PopulationUnchangedCriterion)
Expander1D(geneial::algorithm::stopping_criteria::StatefulStoppingCriterion)

//Choosing
Expander1D(geneial::operation::choosing::ChooseRandom)
Expander1D(geneial::operation::choosing::LambdaChoosingAdapter)

//Mutation
Expander2D(geneial::operation::mutation::SmoothPeakMutationOperation)
Expander2D(geneial::operation::mutation::UniformMutationOperation)
Expander2D(geneial::operation::mutation::ValueSwapMutationOperation)

//Coupling
Expander1D(geneial::operation::coupling::RandomCouplingOperation)
Expander1D(geneial::operation::coupling::SimpleCouplingOperation)
Expander1D(geneial::operation::coupling::LambdaCouplingAdapter)

//Crossover
Expander2D(geneial::operation::crossover::SmoothedMultiValueChromosomeNPointCrossover)
Expander2D(geneial::operation::crossover::MultiValueChromosomeAverageCrossover)
Expander2D(geneial::operation::crossover::MultiValueChromosomeBlendingCrossover)
Expander2D(geneial::operation::crossover::MultiValueChromosomeNPointCrossover)
Expander2D(geneial::operation::crossover::MultiValuePermutationCrossover)

//Replacement
Expander1D(geneial::operation::replacement::ReplaceRandomOperation)
Expander1D(geneial::operation::replacement::ReplaceWorstOperation)

//Selection
Expander1D(geneial::operation::selection::RouletteWheelSelection)
Expander1D(geneial::operation::selection::FitnessProportionalSelection)
Expander1D(geneial::operation::selection::UniformRandomSelection)

//Algorithm
Expander1D(geneial::algorithm::SteadyStateAlgorithm)

//Auxiliary
Expander1D(geneial::algorithm::Diagnostics)

#endif

BOOST_AUTO_TEST_SUITE( __INSTANCIATION_TEST_ALIBI )

BOOST_AUTO_TEST_CASE( Instanciation )
{
    BOOST_CHECK(1 == 1); //when this test compiles it already has reached it's purpose
}
BOOST_AUTO_TEST_SUITE_END()

