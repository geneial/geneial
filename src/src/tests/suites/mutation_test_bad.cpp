#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE operations/mutation

#include <boost/test/unit_test.hpp>

#include <geneial/algorithm/BaseGeneticAlgorithm.h>

#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/fitness/FitnessEvaluator.h>
#include <geneial/core/population/PopulationSettings.h>

#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>

#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/operations/mutation/NonUniformMutationOperation.h>
#include <geneial/core/operations/mutation/UniformMutationOperation.h>
#include <geneial/core/operations/choosing/ChooseRandom.h>

#include "mocks/MockFitnessEvaluator.h"

#include <memory>

using namespace geneial;
using namespace geneial::population::management;
using namespace geneial::operation::choosing;
using namespace geneial::operation::mutation;

using namespace test_mock;

BOOST_AUTO_TEST_SUITE( TESTSUITE_UniformMutationOperation )

//TODO (bewo): Separate Uniform and Nonuniform into separate testcases
//TODO (bewo): Use more mock objects / helper here to avoid tedious duplicate gluecode?

BOOST_AUTO_TEST_CASE( UNIFORM_TEST__basicMutation )
{
    /*
     * 100% Chance of mutation
     * Tests if Chromosomes are actually mutated
     *
     * Test for Uniform and NonUniform Mutation
     */

    MockFitnessEvaluator<double>::ptr evaluator(new MockFitnessEvaluator<double>());
    ContinousMultiValueBuilderSettings<int, double> builderSettings (evaluator, 10, 130, 0, true, 20, 5);
    ContinousMultiValueChromosomeFactory<int, double> chromosomeFactory(builderSettings);
    BaseManager<double> manager(std::make_shared<ContinousMultiValueChromosomeFactory<int, double>>(chromosomeFactory));

    BOOST_TEST_MESSAGE("Checking Mutation at 100% probability");
    for (double i = 0; i <= 1; i = i + 0.1)
    {
        MutationSettings mutationSettings(1, i, 0);
        ChooseRandom<int, double> mutationChoosingOperation(mutationSettings);
        NonUniformMutationOperation<int, double> mutationOperation_NonUniform(1000,
                0.2, mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);
        UniformMutationOperation<int, double> mutationOperation_Uniform (mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

        BaseChromosome<double>::ptr _newChromosome = chromosomeFactory.createChromosome(
                BaseChromosomeFactory<double>::CREATE_VALUES);
        geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
        geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_NonUniform;
        geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_Uniform;

        inputSet.push_back(_newChromosome);
        resultSet_NonUniform.push_back(_newChromosome);
        resultSet_Uniform.push_back(_newChromosome);

        resultSet_NonUniform = mutationOperation_NonUniform.doMutate(inputSet, manager);
        resultSet_Uniform = mutationOperation_Uniform.doMutate(inputSet, manager);

        BOOST_TEST_MESSAGE("Checking at amount of Mutation = "<< i);
        BOOST_CHECK(inputSet != resultSet_NonUniform);
        BOOST_CHECK(inputSet != resultSet_Uniform);
    }

//    BOOST_TEST_MESSAGE ("");
//    BOOST_TEST_MESSAGE ("Checking Mutation at 0% probability");
    for (double i = 0; i <= 1; i = i + 0.1)
    {
        MutationSettings mutationSettings(0, i, 0);
        ChooseRandom<int, double> mutationChoosingOperation(mutationSettings);
        NonUniformMutationOperation<int, double> mutationOperation_NonUniform (1000, 0.2, mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);
        UniformMutationOperation<int, double> mutationOperation_Uniform (mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

        BaseChromosome<double>::ptr _newChromosome = chromosomeFactory.createChromosome(
                BaseChromosomeFactory<double>::CREATE_VALUES);
        geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
        geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_NonUniform;
        geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_Uniform;

        inputSet.push_back(_newChromosome);
        resultSet_NonUniform.push_back(_newChromosome);
        resultSet_Uniform.push_back(_newChromosome);

        resultSet_NonUniform = mutationOperation_NonUniform.doMutate(inputSet, manager);
        resultSet_Uniform = mutationOperation_Uniform.doMutate(inputSet, manager);

        BOOST_TEST_MESSAGE("Checking at amount of Mutation = "<< i);
        BOOST_CHECK(inputSet == resultSet_NonUniform);
        BOOST_CHECK(inputSet == resultSet_Uniform);
    }
}

BOOST_AUTO_TEST_CASE( UNIFORM_TEST__Mutation_probability )
{
    /*
     * Testing Mutation probability for 10000 Testcases
     * Checking UNIFOM and NONUNIFORM mutation
     *
     */

    MockFitnessEvaluator<double>::ptr evaluator(new MockFitnessEvaluator<double>());
    ContinousMultiValueBuilderSettings<int, double> builderSettings(evaluator, 10, 130, 0, true, 20, 5);
    ContinousMultiValueChromosomeFactory<int, double> chromosomeFactory(builderSettings);
    BaseManager<double> manager(std::make_shared<ContinousMultiValueChromosomeFactory<int, double>>(chromosomeFactory));

    //TODO (bewo): Constantify magic numbers...
    for (double probability = 0.0; probability <= 1.0; probability = probability + 0.1)
    {

//        BaseChromosome<double>::ptr _newChromosome = chromosomeFactory->createChromosome(true);
//        BaseMutationOperation<double>::mutation_result_set inputSet;
//        BaseMutationOperation<double>::mutation_result_set resultSet[10000];

        MutationSettings mutationSettings(probability, 1, 5);
        ChooseRandom<int, double> mutationChoosingOperation(mutationSettings);
        NonUniformMutationOperation<int, double> mutationOperation_NonUniform(1000, 0.2, mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

        UniformMutationOperation<int, double> mutationOperation_Uniform(mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

        BaseChromosome<double>::ptr _newChromosome = chromosomeFactory.createChromosome(BaseChromosomeFactory<double>::CREATE_VALUES);
        geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
        geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_NonUniform[10000];
        geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_Uniform[10000];

        inputSet.push_back(_newChromosome);

        int mutationCounter_NonUniform = 0;
        int mutationCounter_Uniform = 0;
        for (int i = 0; i < 10000; i++)
        {
            resultSet_NonUniform[i].push_back(_newChromosome);
            resultSet_NonUniform[i] = mutationOperation_NonUniform.doMutate(inputSet, manager);
            if (inputSet != resultSet_NonUniform[i])
            {
                mutationCounter_NonUniform++;
            }

            resultSet_Uniform[i].push_back(_newChromosome);
            resultSet_Uniform[i] = mutationOperation_Uniform.doMutate(inputSet, manager);
            if (inputSet != resultSet_Uniform[i])
            {
                mutationCounter_Uniform++;
            }
        }

        if (probability < 0)
        {
            //Checking for NON-UNIFORM Mutation
            BOOST_TEST_MESSAGE("Non-Uniform-Mutation porpability: "<< probability);
            BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_NonUniform);
            BOOST_CHECK(mutationCounter_NonUniform == 0);

            //Checking for UNIFORM Mutation
            BOOST_TEST_MESSAGE("Uniform-Mutation porpability: "<< probability);
            BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_Uniform);
            BOOST_CHECK(mutationCounter_Uniform == 0);
        }
        else if (probability > 1)
        {
            //Checking for NON-UNIFORM Mutation
            BOOST_CHECK(mutationCounter_NonUniform = 10000);

            BOOST_TEST_MESSAGE("Non-Uniform-Mutation porpability: "<< probability);
            BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_NonUniform);

            //Checking for UNIFORM Mutation
            BOOST_CHECK(mutationCounter_Uniform = 10000);

            BOOST_TEST_MESSAGE("Uniform-Mutation porpability: "<< probability);
            BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_Uniform);
        }
        else
        {
            //Checking for NON-UNIFORM Mutation
            BOOST_CHECK(mutationCounter_NonUniform > (10000 * probability - 200));
            BOOST_CHECK(mutationCounter_NonUniform < (10000 * probability + 200));

            BOOST_TEST_MESSAGE("Non-Uniform-Mutation porpability: "<< probability);
            BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_NonUniform);

            //Checking for UNIFORM Mutation
            BOOST_CHECK(mutationCounter_Uniform > (10000 * probability - 200));
            BOOST_CHECK(mutationCounter_Uniform < (10000 * probability + 200));

            BOOST_TEST_MESSAGE("Uniform-Mutation porpability: "<< probability);
            BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_Uniform);
        }
    }
}

BOOST_AUTO_TEST_CASE ( UNIFORM_TEST__points_of_mutation )
{
    /*
     * Checking if as many points are mutated as set in Mutation settings
     */

    MockFitnessEvaluator<double>::ptr evaluator(new MockFitnessEvaluator<double>());
    ContinousMultiValueBuilderSettings<int, double> builderSettings (evaluator, 100, 130, 0, true, 20, 5);
    ContinousMultiValueChromosomeFactory<int, double> chromosomeFactory (builderSettings);
    BaseManager<double> manager(std::make_shared<ContinousMultiValueChromosomeFactory<int, double>>(chromosomeFactory));

    for (unsigned int pointsOfMutation = 0; pointsOfMutation <= 102; pointsOfMutation++)
    {
        MutationSettings mutationSettings(1, 1, pointsOfMutation);
        ChooseRandom<int, double> mutationChoosingOperation(mutationSettings);

        NonUniformMutationOperation<int, double> mutationOperation_NonUniform (1000,
                0.2, mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

        UniformMutationOperation<int, double> mutationOperation_Uniform(mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

        BaseChromosome<double>::ptr _newChromosome = chromosomeFactory.createChromosome(
                BaseChromosomeFactory<double>::CREATE_VALUES);

        geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
        geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_NonUniform;
        geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_Uniform;

        inputSet.push_back(_newChromosome);
        resultSet_NonUniform.push_back(_newChromosome);
        resultSet_Uniform.push_back(_newChromosome);

        resultSet_NonUniform = mutationOperation_NonUniform.doMutate(inputSet, manager);
        resultSet_Uniform = mutationOperation_Uniform.doMutate(inputSet, manager);

        //getting Chromosome from resultSet:
        MultiValueChromosome<int, double>::ptr mvcMutant_NonUniform = std::dynamic_pointer_cast<
                MultiValueChromosome<int, double> >(*resultSet_NonUniform.begin());

        MultiValueChromosome<int, double>::ptr mvcMutant_Uniform = std::dynamic_pointer_cast<
                MultiValueChromosome<int, double> >(*resultSet_Uniform.begin());

        MultiValueChromosome<int, double>::ptr mvcOriginal = std::dynamic_pointer_cast<
                MultiValueChromosome<int, double> >(*inputSet.begin());

        //getting ValueContainer from Chromosome:
        MultiValueChromosome<int, double>::value_container &mvcMutant_NonUniform_valueContainer =
                mvcMutant_NonUniform->getContainer();

        MultiValueChromosome<int, double>::value_container &mvcMutant_Uniform_valueContainer =
                mvcMutant_Uniform->getContainer();

        MultiValueChromosome<int, double>::value_container &mvcOriginal_valueContainer = mvcOriginal->getContainer();

        //setting Iterators
        MultiValueChromosome<int, double>::value_container::iterator original_it = mvcOriginal_valueContainer.begin();

        unsigned int nunUniformdiffCounter = 0;

        for (MultiValueChromosome<int, double>::value_container::iterator nonUniformMutant_it =
                mvcMutant_NonUniform_valueContainer.begin();
                nonUniformMutant_it != mvcMutant_NonUniform_valueContainer.end(); ++nonUniformMutant_it)
        {
            //BOOST_TEST_MESSAGE(original_it);
            if (*original_it != *nonUniformMutant_it)
            {
                nunUniformdiffCounter++;
            }
            ++original_it;
        }

        BOOST_TEST_MESSAGE("");
        BOOST_TEST_MESSAGE(
                "Check if as many points were Mutated as specified in MutationSettings: "<< pointsOfMutation);
        BOOST_TEST_MESSAGE("NON-UNIFORM: "<< nunUniformdiffCounter);

        if (pointsOfMutation != 0)
        {
            BOOST_CHECK(
                    (nunUniformdiffCounter <= pointsOfMutation + 5 && pointsOfMutation <= 100)
                            || (nunUniformdiffCounter <= 100 && pointsOfMutation > 100));
        }
        else
        {
            BOOST_CHECK(nunUniformdiffCounter >= 90);
        }

        unsigned int uniformdiffCounter = 0;
        original_it = mvcOriginal_valueContainer.begin();
        for (MultiValueChromosome<int, double>::value_container::iterator uniformMutant_it =
                mvcMutant_Uniform_valueContainer.begin(); uniformMutant_it != mvcMutant_Uniform_valueContainer.end();
                ++uniformMutant_it)
        {
            if (*original_it != *uniformMutant_it)
                uniformdiffCounter++;
            ++original_it;
        }

        BOOST_TEST_MESSAGE("UNIFORM: "<< uniformdiffCounter);
        if (pointsOfMutation != 0)
        {
            BOOST_CHECK(
                    (uniformdiffCounter <= pointsOfMutation + 5 && pointsOfMutation <= 100)
                            || (uniformdiffCounter <= 100 && pointsOfMutation > 100));
        }
        else
        {
            BOOST_CHECK(uniformdiffCounter >= 90);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( TESTSUITE_SmoothingMutation )

//Checks whether the smoothing mutation destroys chromosome's smoothness
BOOST_AUTO_TEST_CASE ( SMOOTHING_TEST__ensure_nonviolated_smoothness )
{

}

//Test whether mutation generates values above/below minmax
BOOST_AUTO_TEST_CASE ( SMOOTHING_TEST__ensure_adherence_min_max )
{

}

//Test Peak at chromosome borders.

BOOST_AUTO_TEST_SUITE_END()
