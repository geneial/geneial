#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE compilation/template_instanciaton

#include <boost/test/unit_test.hpp>

/**
 * @brief Initialization is used to ensure all the geneial code compiles,
 * regardless of whether it is being used in some demo program or not.
 *
 * This Test ensures the whole library is kept at a compiling state, even
 * if there are functions that are not currently being utilized by some test
 * program, benchmark or demo. Those functions might be used by some client which
 * then will discover the compile time error.
 *
 * This test is only compiled in the Debug mode.
 *
 * This test can be compiled "exhaustively" to ensure max. compability
 * with int, double, float instanciations of all templates.
 *
 */

#ifndef NDEBUG

#include <geneial/algorithm/SteadyStateAlgorithm.h>

#include <geneial/core/operations/choosing/ChooseRandom.h>

#include <geneial/core/operations/mutation/NonUniformMutationOperation.h>
#include <geneial/core/operations/mutation/SmoothPeakMutationOperation.h>
#include <geneial/core/operations/mutation/UniformMutationOperation.h>

#include <geneial/core/operations/coupling/SimpleCouplingOperation.h>
#include <geneial/core/operations/coupling/RandomCouplingOperation.h>


#include <geneial/core/operations/crossover/MultiValueChromosomeAverageCrossover.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeBlendingCrossover.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/core/operations/crossover/SmoothedMultiValueChromosomeNPointCrossover.h>


#include <geneial/core/operations/replacement/ReplaceRandomOperation.h>
#include <geneial/core/operations/replacement/ReplaceWorstOperation.h>

#include <geneial/core/operations/selection/FitnessProportionalSelection.h>
#include <geneial/core/operations/selection/RouletteWheelSelection.h>
#include <geneial/core/operations/selection/UniformRandomSelection.h>


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

//Algorithm
Expander1D(geneial::algorithm::SteadyStateAlgorithm)

//Choosing
Expander2D(geneial::operation::choosing::ChooseRandom);

//Mutation
Expander2D(geneial::operation::mutation::NonUniformMutationOperation)
Expander2D(geneial::operation::mutation::SmoothPeakMutationOperation)
Expander2D(geneial::operation::mutation::UniformMutationOperation)

//Coupling
Expander1D(geneial::operation::coupling::RandomCouplingOperation)
Expander1D(geneial::operation::coupling::SimpleCouplingOperation)

//Crossover
Expander2D(geneial::operation::crossover::SmoothedMultiValueChromosomeNPointCrossover)
Expander2D(geneial::operation::crossover::MultiValueChromosomeAverageCrossover)
Expander2D(geneial::operation::crossover::MultiValueChromosomeBlendingCrossover)
Expander2D(geneial::operation::crossover::MultiValueChromosomeNPointCrossover)

//Replacement
Expander1D(geneial::operation::replacement::ReplaceRandomOperation);
Expander1D(geneial::operation::replacement::ReplaceWorstOperation);

//Selection
Expander1D(geneial::operation::selection::RouletteWheelSelection);
Expander1D(geneial::operation::selection::FitnessProportionalSelection);
Expander1D(geneial::operation::selection::UniformRandomSelection);

#endif


BOOST_AUTO_TEST_SUITE( __INSTANCIATION_TEST_ALIBI );
BOOST_AUTO_TEST_CASE( Instanciation )
{
    BOOST_CHECK(1==1); //when this test compiles it already has reached it's purpose
}
BOOST_AUTO_TEST_SUITE_END()

