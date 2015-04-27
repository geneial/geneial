#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE contiuous_mvc

#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include <set>
#include <vector>

#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>

#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>
#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>

#include "mocks/MockFitnessEvaluator.h"

#include "helper/common_casts.h"
#include "helper/continuity.h"

using namespace geneial::population::chromosome;

BOOST_AUTO_TEST_SUITE( ContinousMultiValueChromosome )

#define CONTINUITY_FACTORY_SIGNIFICANCE (100);
BOOST_AUTO_TEST_CASE( Continuity_Factory )
{
    const test_mock::MockDoubleFitnessEvaluator::ptr evaluator(new test_mock::MockDoubleFitnessEvaluator);

    int i = CONTINUITY_FACTORY_SIGNIFICANCE
    ;
    while (i--)
    {
        BOOST_MESSAGE("130-0,20,5");
        ContinousMultiValueBuilderSettings<int, double> builderSettings1(evaluator, 2, 130, 0, true, 20, 5);
        ContinousMultiValueChromosomeFactory<int, double> chromosomeFactory1(builderSettings1);
        BaseChromosome<double>::ptr testCandidate11 = chromosomeFactory1.createChromosome();
        test_helper::checkContinuity<int, double>(testCandidate11, 5);
        test_helper::checkLimits<int, double>(testCandidate11, 0, 130);

        BaseChromosome<double>::ptr testCandidate12 = chromosomeFactory1.createChromosome();
        test_helper::checkContinuity<int, double>(testCandidate12, 5);
        test_helper::checkLimits<int, double>(testCandidate12, 0, 130);

        BOOST_MESSAGE("130-0,20,1");
        ContinousMultiValueBuilderSettings<int, double> builderSettings2(evaluator, 1, 130, 0, true, 20, 1);
        ContinousMultiValueChromosomeFactory<int, double> chromosomeFactory2(builderSettings2);
        BaseChromosome<double>::ptr testCandidate2 = chromosomeFactory2.createChromosome();
        test_helper::checkContinuity<int, double>(testCandidate2, 1);
        test_helper::checkLimits<int, double>(testCandidate2, 0, 130);

        BOOST_MESSAGE("130-0,0,5");
        ContinousMultiValueBuilderSettings<int, double> builderSettings3(evaluator, 1, 130, 0, false, 0, 5);
        ContinousMultiValueChromosomeFactory<int, double> chromosomeFactory3 (builderSettings3);
        BaseChromosome<double>::ptr testCandidate3 = chromosomeFactory3.createChromosome();
        test_helper::checkContinuity<int, double>(testCandidate3, 5);
        test_helper::checkLimits<int, double>(testCandidate3, 0, 130);

        BOOST_MESSAGE("-130-0,0,5");
        ContinousMultiValueBuilderSettings<int, double> builderSettings4(evaluator, 1, 0, -130, false, 0, 5);
        ContinousMultiValueChromosomeFactory<int, double> chromosomeFactory4(builderSettings4);
        BaseChromosome<double>::ptr testCandidate4 = chromosomeFactory4.createChromosome();
        test_helper::checkContinuity<int, double>(testCandidate4, 5);
        test_helper::checkLimits<int, double>(testCandidate4, -130, 0);

        BOOST_MESSAGE("-130-130,0,5");
        ContinousMultiValueBuilderSettings<int, double> builderSettings5(evaluator, 1, 130, -130, false, 0, 5);
        ContinousMultiValueChromosomeFactory<int, double> chromosomeFactory5(builderSettings5);
        BaseChromosome<double>::ptr testCandidate5 = chromosomeFactory5.createChromosome();
        test_helper::checkContinuity<int, double>(testCandidate5, 5);
        test_helper::checkLimits<int, double>(testCandidate5, -130, 130);

        BOOST_MESSAGE("130-0,0,5");
        ContinousMultiValueBuilderSettings<int, double> builderSettings6(evaluator, 1, 130, 0, false, 0, 130);
        ContinousMultiValueChromosomeFactory<int,double> chromosomeFactory6(builderSettings6);
        BaseChromosome<double>::ptr testCandidate6 = chromosomeFactory6.createChromosome();
        test_helper::checkContinuity<int, double>(testCandidate6, 130);
        test_helper::checkLimits<int, double>(testCandidate6, -130, 130);
    }
}

BOOST_AUTO_TEST_SUITE_END()
