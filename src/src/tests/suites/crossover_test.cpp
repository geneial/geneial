#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE algorithm/crossover

#include <boost/test/unit_test.hpp>

#include <set>
#include <vector>

#include <geneial/algorithm/BaseGeneticAlgorithm.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>

#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>
#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>

#include <geneial/core/operations/crossover/MultiValueChromosomeBlendingCrossover.h>

#include "mocks/MockFitnessEvaluator.h"

#include "helper/common_casts.h"
#include "helper/continuity.h"
#include "helper/chromosome_compare.h"


using ::geneial::operation::crossover::MultiValueChromosomeBlendingCrossover;

using InterpolateBeta = ::geneial::operation::crossover::MultiValueChromosomeBlendingCrossover<int,double>::InterpolateBeta;

struct mvc_blending_testsets
{
    InterpolateBeta interpolationMethod;
    int eps;
    int num_vals;
    int random_min;
    int random_max;
    bool hasStart;
    int start;

};
const int CONTINUITY_BLENDING_SIGNIFICANCE = 1000;
BOOST_AUTO_TEST_SUITE( __CROSSOVER_MVCBlendingCrossover )
BOOST_AUTO_TEST_CASE( ContinuityBlending )
{
    const test_mock::MockFitnessEvaluator<double>::ptr evaluator(new test_mock::MockFitnessEvaluator<double>);

       std::vector<mvc_blending_testsets> blending_testsets = {
               { InterpolateBeta::INTERPOLATE_LINEARLY, 1, 50, 0, 100, true, 1},
               { InterpolateBeta::INTERPOLATE_LINEARLY, 1, 50, 0, 100, true, 100},
               { InterpolateBeta::INTERPOLATE_LINEARLY, 2, 50, 0, 100, false,0},
               { InterpolateBeta::INTERPOLATE_LINEARLY, 2, 50, -100, 100, false, 0},
               { InterpolateBeta::INTERPOLATE_LINEARLY, 2, 50, -100, 0, false, 0},
               { InterpolateBeta::INTERPOLATE_LINEARLY, 10, 5, 0, 10, false, 0},
               { InterpolateBeta::INTERPOLATE_LINEARLY, 10, 5, 0, 10, true, 5},

               { InterpolateBeta::INTERPOLATE_NONE, 1, 50, 0, 100, true, 1},
               { InterpolateBeta::INTERPOLATE_NONE, 1, 50, 0, 100, true, 100},
               { InterpolateBeta::INTERPOLATE_NONE, 2, 50, 0, 100, false,0},
               { InterpolateBeta::INTERPOLATE_NONE, 2, 50, -100, 100, false, 0},
               { InterpolateBeta::INTERPOLATE_NONE, 2, 50, -100, 0, false, 0},
               { InterpolateBeta::INTERPOLATE_NONE, 10, 5, 0, 10, false, 0},
               { InterpolateBeta::INTERPOLATE_NONE, 10, 5, 0, 10, true, 5},

               { InterpolateBeta::INTERPOLATE_COSINE, 1, 50, 0, 100, true, 1},
               { InterpolateBeta::INTERPOLATE_COSINE, 1, 50, 0, 100, true, 100},
               { InterpolateBeta::INTERPOLATE_COSINE, 2, 50, 0, 100, false,0},
               { InterpolateBeta::INTERPOLATE_COSINE, 2, 50, -100, 100, false, 0},
               { InterpolateBeta::INTERPOLATE_COSINE, 2, 50, -100, 0, false, 0},
               { InterpolateBeta::INTERPOLATE_COSINE, 10, 5, 0, 10, false, 0},
               { InterpolateBeta::INTERPOLATE_COSINE, 10, 5, 0, 10, true, 5},

               { InterpolateBeta::INTERPOLATE_RANDOM, 1, 50, 0, 100, true, 1},
               { InterpolateBeta::INTERPOLATE_RANDOM, 1, 50, 0, 100, true, 100},
               { InterpolateBeta::INTERPOLATE_RANDOM, 2, 50, 0, 100, false,0},
               { InterpolateBeta::INTERPOLATE_RANDOM, 2, 50, -100, 100, false, 0},
               { InterpolateBeta::INTERPOLATE_RANDOM, 2, 50, -100, 0, false, 0},
               { InterpolateBeta::INTERPOLATE_RANDOM, 10, 5, 0, 10, false, 0},
               { InterpolateBeta::INTERPOLATE_RANDOM, 10, 5, 0, 10, true, 5}
       };

       int i = CONTINUITY_BLENDING_SIGNIFICANCE;

       while(i--)
       {
           for(const auto& testset : blending_testsets)
           {
               auto builder = ContinousMultiValueChromosomeFactory<int,double>::Builder(evaluator);

               auto factory = std::dynamic_pointer_cast<ContinousMultiValueChromosomeFactory<int,double>>(builder.create());

               builder.getSettings().setNum(testset.num_vals);
               builder.getSettings().setEps(testset.eps);
               builder.getSettings().setRandomMin(testset.random_min);
               builder.getSettings().setRandomMax(testset.random_max);
               builder.getSettings().setHasStart(testset.hasStart);
               builder.getSettings().setStartValue(testset.start);

               auto chromosomeA = factory->createChromosome();
               auto chromosomeB = factory->createChromosome();

               int trials = 100;
               while(trials-- && !test_helper::chromosomeCompare<int,double>(chromosomeA,chromosomeB))
               {
                   chromosomeB = factory->createChromosome();
               }
               const bool compareDifferentAB = test_helper::chromosomeCompare<int,double>(chromosomeA,chromosomeB);
               BOOST_ASSERT(!compareDifferentAB);

//               std::cout << *chromosomeA <<std::endl;
//               std::cout << *chromosomeB <<std::endl;

               test_helper::checkContinuity<int,double>(chromosomeA,testset.eps);
               test_helper::checkContinuity<int,double>(chromosomeB,testset.eps);

               auto crossoverOperationBuilder = ::geneial::operation::crossover::MultiValueChromosomeBlendingCrossover<int,double>::Builder(factory);
               crossoverOperationBuilder.setNumChilds(1);
               crossoverOperationBuilder.setInterpolationMethod(testset.interpolationMethod);
               auto operation = crossoverOperationBuilder.create();

               auto childcontainer = operation->doCrossover(chromosomeA,chromosomeB);
               BOOST_CHECK(childcontainer.size() == 1);

               //differs from mommie/daddy?
               const bool compareDifferentA = test_helper::chromosomeCompare<int,double>(childcontainer[0],chromosomeA);
               BOOST_ASSERT(!compareDifferentA);
               const bool compareDifferentB = test_helper::chromosomeCompare<int,double>(childcontainer[0],chromosomeB);
               BOOST_ASSERT(!compareDifferentB);

               if(testset.interpolationMethod != InterpolateBeta::INTERPOLATE_NONE)
               {
                   //Continuity preserving?
                   //TODO (bewo): Bug, Blending does not preserve continuity.
                   test_helper::checkContinuity<int,double>(childcontainer[0],testset.eps);
                   //std::cout << *childcontainer[0] << std::endl;
                   if(testset.hasStart)
                   {
                       //TODO (bewo): Blending ignores start value
                       //test_helper::checkForStart<int,double>(childcontainer[0],testset.start,testset.eps);
                   }
               }
               test_helper::checkLimits<int,double>(childcontainer[0],testset.random_min,testset.random_max);
           }
       }


}
BOOST_AUTO_TEST_SUITE_END()
