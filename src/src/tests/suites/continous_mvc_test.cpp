#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE contiuous_mvc

#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include <set>
#include <vector>

#include <geneial/algorithm/BaseGeneticAlgorithm.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>

#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>
#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>

#include "mocks/MockFitnessEvaluator.h"

#include "helper/common_casts.h"
#include "helper/continuity.h"
#include "helper/chromosome_compare.h"

using namespace geneial::population::chromosome;

BOOST_AUTO_TEST_SUITE( ContinousMultiValueChromosome )

const int CONTINUITY_FACTORY_SIGNIFICANCE = 1000;


struct continuity_factory_testsets //POD struct for testcases for the following test
{
    int amountChromosomes;
    int epsilon;
    int min;
    int max;
    bool hasStart;
    int start;
};

BOOST_AUTO_TEST_CASE( Continuity_Factory )
{
    const test_mock::MockFitnessEvaluator<double>::ptr evaluator(new test_mock::MockFitnessEvaluator<double>);

    int i = CONTINUITY_FACTORY_SIGNIFICANCE;

    std::vector<continuity_factory_testsets> testcases = {
            {100,5,0,130,true,20},
            {100,5,0,130,false,0},
            {100,1,0,130,true,20},
            {100,1,0,130,false,0},
            {100,5,0,130,true,0},
            {100,5,0,130,false,0},
            {100,5,0,130,true,5},
            {100,5,-130,130,true,5},
            {100,5,-130,130,false,0},
            {100,5,-130,-10,true,-5},
            {100,5,-130,-10,false,0},
            {100,5,-130,0,true,-5},
            {100,5,-130,0,false,0},
            {100,5,-130,0,true,-5},
            {100,5,-130,0,false,0}
    };


    while (i--)
    {
        for(const auto &testcase :testcases)
        {

            auto builder = ContinousMultiValueChromosomeFactory<int,double>::Builder(evaluator);
            auto factory = std::dynamic_pointer_cast<ContinousMultiValueChromosomeFactory<int,double>>(builder.create());
            builder.getSettings().setNum(testcase.amountChromosomes);
            builder.getSettings().setEps(testcase.epsilon);
            builder.getSettings().setRandomMin(testcase.min);
            builder.getSettings().setRandomMax(testcase.max);
            builder.getSettings().setHasStart(testcase.hasStart);
            builder.getSettings().setStartValue(testcase.start);

            auto chromosome = factory->createChromosome();
            test_helper::checkContinuity<int, double>(chromosome, testcase.epsilon);
            test_helper::checkLimits<int, double>(chromosome, testcase.min, testcase.max);

            if(testcase.hasStart)
            {
                test_helper::checkForStart<int,double>(chromosome,testcase.start,testcase.epsilon);
            }
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()
