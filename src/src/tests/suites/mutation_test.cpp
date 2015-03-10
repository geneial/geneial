#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE operations/mutation

#include "boost/shared_ptr.hpp"
#include <boost/test/unit_test.hpp>

#include <geneial/algorithm/BaseGeneticAlgorithm.h>

#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/fitness/FitnessEvaluator.h>
#include <geneial/core/population/PopulationSettings.h>

#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>
#include <geneial/core/population/builder/ContinousMultiIntValueChromosomeFactory.h>

#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/operations/mutation/NonUniformMutationOperation.h>
#include <geneial/core/operations/mutation/UniformMutationOperation.h>
#include <geneial/core/operations/choosing/ChooseRandom.h>

#include "mocks/MockFitnessEvaluator.h"

using namespace geneial;
using namespace geneial::population::management;
using namespace geneial::operation::choosing;
using namespace geneial::operation::mutation;

using namespace test_mock;

BOOST_AUTO_TEST_SUITE( __MUTATION_TEST_UniformMutationOperation )


BOOST_AUTO_TEST_CASE( __MUTATION_TESTbasicMutation )
{
	/*100% Chance of mutation
	 * Tests if Chromosomes are actually mutated
	 *
	 * Test for Uniform and NonUniform Mutation
	 */


	MockDoubleFitnessEvaluator::ptr evaluator(new MockDoubleFitnessEvaluator());
	PopulationSettings *populationSettings = new PopulationSettings(50);
	ContinousMultiValueBuilderSettings<int,double> *builderSettings = new ContinousMultiValueBuilderSettings<int,double>(evaluator,10,130,0,true,20,5);
	ContinousMultiIntValueChromosomeFactory<double> *chromosomeFactory = new ContinousMultiIntValueChromosomeFactory<double>(builderSettings);

	BaseManager<double> manager(populationSettings, chromosomeFactory);

	BOOST_TEST_MESSAGE ("Checking Mutation at 100% probability");
	for (double i = 0; i <= 1; i = i + 0.1)
	{
		MutationSettings* mutationSettings = new MutationSettings(1,i,0);
		ChooseRandom<int,double> *mutationChoosingOperation = new ChooseRandom<int,double>(mutationSettings);
		BaseMutationOperation<double> *mutationOperation_NonUniform = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);
		BaseMutationOperation<double> *mutationOperation_Uniform = new UniformMutationOperation<int,double>(mutationSettings,mutationChoosingOperation,builderSettings,chromosomeFactory);

		BaseChromosome<double>::ptr _newChromosome = chromosomeFactory->createChromosome(BaseChromosomeFactory<double>::CREATE_VALUES);
		geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
		geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_NonUniform;
		geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_Uniform;

		inputSet.push_back(_newChromosome);
		resultSet_NonUniform.push_back(_newChromosome);
		resultSet_Uniform.push_back(_newChromosome);


		resultSet_NonUniform = mutationOperation_NonUniform->doMutate(inputSet,manager);
		resultSet_Uniform = mutationOperation_Uniform->doMutate(inputSet,manager);

		BOOST_TEST_MESSAGE ("Checking at amount of Mutation = "<< i);
		BOOST_CHECK(inputSet != resultSet_NonUniform);
		BOOST_CHECK(inputSet != resultSet_Uniform);
	}

//	BOOST_TEST_MESSAGE ("");
//	BOOST_TEST_MESSAGE ("Checking Mutation at 0% probability");
	for (double i = 0; i <= 1; i = i + 0.1)
	{
		MutationSettings* mutationSettings = new MutationSettings(0,i,0);
		ChooseRandom<int,double> *mutationChoosingOperation = new ChooseRandom<int,double>(mutationSettings);
		BaseMutationOperation<double> *mutationOperation_NonUniform = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);
		BaseMutationOperation<double> *mutationOperation_Uniform = new UniformMutationOperation<int,double>(mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

//	BaseChromosome<double>::ptr _newChromosome = chromosomeFactory->createChromosome(true);
//	BaseMutationOperation<double>::mutation_result_set inputSet;
//	BaseMutationOperation<double>::mutation_result_set resultSet;


		BaseChromosome<double>::ptr _newChromosome = chromosomeFactory->createChromosome(BaseChromosomeFactory<double>::CREATE_VALUES);
		geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
		geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_NonUniform;
		geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_Uniform;

		inputSet.push_back(_newChromosome);
		resultSet_NonUniform.push_back(_newChromosome);
		resultSet_Uniform.push_back(_newChromosome);

		resultSet_NonUniform = mutationOperation_NonUniform->doMutate(inputSet,manager);
		resultSet_Uniform = mutationOperation_Uniform->doMutate(inputSet,manager);

		BOOST_TEST_MESSAGE ("Checking at amount of Mutation = "<< i);
		BOOST_CHECK(inputSet == resultSet_NonUniform);
		BOOST_CHECK(inputSet == resultSet_Uniform);
	}
}


BOOST_AUTO_TEST_CASE( __MUTATION_TEST_Mutation_Propability )
{
	/*
	 * Testing Mutation Propability for 10000 Testcases
	 * Checking UNIFOM and NONUNIFORM mutation
	 *
	 */


	MockDoubleFitnessEvaluator::ptr evaluator(new MockDoubleFitnessEvaluator());
	PopulationSettings *populationSettings = new PopulationSettings(50);
	ContinousMultiValueBuilderSettings<int,double> *builderSettings = new ContinousMultiValueBuilderSettings<int,double>(evaluator,10,130,0,true,20,5);
	ContinousMultiIntValueChromosomeFactory<double> *chromosomeFactory = new ContinousMultiIntValueChromosomeFactory<double>(builderSettings);

	BaseManager<double> manager(populationSettings, chromosomeFactory);


	for (double propability = 0.0; propability <= 1.0; propability = propability + 0.1)
	{

		BOOST_TEST_MESSAGE("");


//		BaseChromosome<double>::ptr _newChromosome = chromosomeFactory->createChromosome(true);
//		BaseMutationOperation<double>::mutation_result_set inputSet;
//		BaseMutationOperation<double>::mutation_result_set resultSet[10000];


		MutationSettings* mutationSettings = new MutationSettings(propability,1,5);
		ChooseRandom<int,double> *mutationChoosingOperation = new ChooseRandom<int,double>(mutationSettings);
		BaseMutationOperation<double> *mutationOperation_NonUniform = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);
		BaseMutationOperation<double> *mutationOperation_Uniform = new UniformMutationOperation<int,double>(mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

		BaseChromosome<double>::ptr _newChromosome = chromosomeFactory->createChromosome(BaseChromosomeFactory<double>::CREATE_VALUES);
		geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
		geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_NonUniform[10000];
		geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_Uniform[10000];

		inputSet.push_back(_newChromosome);

		int mutationCounter_NonUniform = 0;
		int mutationCounter_Uniform = 0;
		for (int i = 0; i < 10000; i++)
		{
			resultSet_NonUniform[i].push_back(_newChromosome);
			resultSet_NonUniform[i] = mutationOperation_NonUniform->doMutate(inputSet,manager);
			if (inputSet != resultSet_NonUniform[i]) mutationCounter_NonUniform++;

			resultSet_Uniform[i].push_back(_newChromosome);
			resultSet_Uniform[i] = mutationOperation_Uniform->doMutate(inputSet,manager);
			if (inputSet != resultSet_Uniform[i]) mutationCounter_Uniform++;
		}


		if (propability < 0)
		{
			//Checking for NON-UNIFORM Mutation
			BOOST_TEST_MESSAGE("Non-Uniform-Mutation porpability: "<< propability);
			BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_NonUniform);
			BOOST_CHECK(mutationCounter_NonUniform == 0);

			//Checking for UNIFORM Mutation
			BOOST_TEST_MESSAGE("Uniform-Mutation porpability: "<< propability);
			BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_Uniform);
			BOOST_CHECK(mutationCounter_Uniform == 0);
		}
		else if (propability > 1)
		{
			//Checking for NON-UNIFORM Mutation
			BOOST_CHECK(mutationCounter_NonUniform = 10000);

			BOOST_TEST_MESSAGE("Non-Uniform-Mutation porpability: "<< propability);
			BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_NonUniform);

			//Checking for UNIFORM Mutation
			BOOST_CHECK(mutationCounter_Uniform = 10000);

			BOOST_TEST_MESSAGE("Uniform-Mutation porpability: "<< propability);
			BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_Uniform);
		}
		else
		{
			//Checking for NON-UNIFORM Mutation
			BOOST_CHECK(mutationCounter_NonUniform > (10000*propability-200));
			BOOST_CHECK(mutationCounter_NonUniform < (10000*propability+200));

			BOOST_TEST_MESSAGE("Non-Uniform-Mutation porpability: "<< propability);
			BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_NonUniform);

			//Checking for UNIFORM Mutation
			BOOST_CHECK(mutationCounter_Uniform > (10000*propability-200));
			BOOST_CHECK(mutationCounter_Uniform < (10000*propability+200));

			BOOST_TEST_MESSAGE("Uniform-Mutation porpability: "<< propability);
			BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_Uniform);
		}
	}
}

BOOST_AUTO_TEST_CASE ( __MUTATION_TEST_points_of_mutation )
{
	/*
	 * Checking if as many points are mutated as set in Mutation settings
	 */

	MockDoubleFitnessEvaluator::ptr evaluator(new MockDoubleFitnessEvaluator());
	PopulationSettings *populationSettings = new PopulationSettings(50);
	ContinousMultiValueBuilderSettings<int,double> *builderSettings = new ContinousMultiValueBuilderSettings<int,double>(evaluator,100,130,0,true,20,5);
	ContinousMultiIntValueChromosomeFactory<double> *chromosomeFactory = new ContinousMultiIntValueChromosomeFactory<double>(builderSettings);

	BaseManager<double> manager(populationSettings, chromosomeFactory);

	for (unsigned int pointsOfMutation = 0; pointsOfMutation <= 102; pointsOfMutation++)
	{
		MutationSettings* mutationSettings = new MutationSettings(1,1,pointsOfMutation);
		ChooseRandom<int,double> *mutationChoosingOperation = new ChooseRandom<int,double>(mutationSettings);

		BaseMutationOperation<double> *mutationOperation_NonUniform = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);
		BaseMutationOperation<double> *mutationOperation_Uniform = new UniformMutationOperation<int,double>(mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

		BaseChromosome<double>::ptr _newChromosome = chromosomeFactory->createChromosome(BaseChromosomeFactory<double>::CREATE_VALUES);
		geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
		geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_NonUniform;
		geneial::operation::mutation::BaseMutationOperation<double>::mutation_result_set resultSet_Uniform;

		inputSet.push_back(_newChromosome);
		resultSet_NonUniform.push_back(_newChromosome);
		resultSet_Uniform.push_back(_newChromosome);

		resultSet_NonUniform = mutationOperation_NonUniform->doMutate(inputSet,manager);
		resultSet_Uniform = mutationOperation_Uniform->doMutate(inputSet,manager);

		//getting Chromosome from resultSet:
		MultiValueChromosome<int,double>::ptr mvcMutant_NonUniform = boost::dynamic_pointer_cast<MultiValueChromosome<int,double> >(*resultSet_NonUniform.begin());
		MultiValueChromosome<int,double>::ptr mvcMutant_Uniform = boost::dynamic_pointer_cast<MultiValueChromosome<int,double> >(*resultSet_Uniform.begin());
		MultiValueChromosome<int,double>::ptr mvcOriginal = boost::dynamic_pointer_cast<MultiValueChromosome<int,double> >(*inputSet.begin());

		//getting ValueContainer from Chromosome:
		MultiValueChromosome<int,double>::value_container &mvcMutant_NonUniform_valueContainer = mvcMutant_NonUniform->getContainer();
		MultiValueChromosome<int,double>::value_container &mvcMutant_Uniform_valueContainer = mvcMutant_Uniform->getContainer();
		MultiValueChromosome<int,double>::value_container &mvcOriginal_valueContainer = mvcOriginal->getContainer();

		//setting Iterators
		MultiValueChromosome<int,double>::value_container::iterator original_it = mvcOriginal_valueContainer.begin();

		unsigned int nunUniformdiffCounter = 0;

		for (MultiValueChromosome<int,double>::value_container::iterator nonUniformMutant_it = mvcMutant_NonUniform_valueContainer.begin();
				nonUniformMutant_it != mvcMutant_NonUniform_valueContainer.end();
				++nonUniformMutant_it)
		{
			//BOOST_TEST_MESSAGE(original_it);
			if (*original_it != *nonUniformMutant_it) nunUniformdiffCounter++;
			++original_it;
		}

		BOOST_TEST_MESSAGE("");
		BOOST_TEST_MESSAGE("Check if as many points were Mutated as specified in MutationSettings: "<< pointsOfMutation);
		BOOST_TEST_MESSAGE("NON-UNIFORM: "<< nunUniformdiffCounter);

		if (pointsOfMutation != 0) BOOST_CHECK((nunUniformdiffCounter <= pointsOfMutation+5 && pointsOfMutation <= 100) || (nunUniformdiffCounter <= 100 && pointsOfMutation > 100));
		else BOOST_CHECK(nunUniformdiffCounter >= 90);

		unsigned int uniformdiffCounter = 0;
		original_it = mvcOriginal_valueContainer.begin();
		for (MultiValueChromosome<int,double>::value_container::iterator uniformMutant_it = mvcMutant_Uniform_valueContainer.begin();
				uniformMutant_it != mvcMutant_Uniform_valueContainer.end();
				++uniformMutant_it)
		{
			if (*original_it != *uniformMutant_it) uniformdiffCounter++;
			++original_it;
		}

		BOOST_TEST_MESSAGE("UNIFORM: "<< uniformdiffCounter);
		if (pointsOfMutation != 0)
		{
			BOOST_CHECK((uniformdiffCounter <= pointsOfMutation+5 && pointsOfMutation <= 100) || (uniformdiffCounter <= 100 && pointsOfMutation > 100));
		}
		else
		{
			BOOST_CHECK(uniformdiffCounter >= 90);
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()

