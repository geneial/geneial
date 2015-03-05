/*
 * mutation_test.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: lukas
 */
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
#include <geneial/core/operations/choosing/ChooseRandom.h>



BOOST_AUTO_TEST_SUITE( UniformMutationOperation )

	using namespace GeneticLibrary;
	using namespace GeneticLibrary::Algorithm;
	using namespace GeneticLibrary::Algorithm::StoppingCriteria;
	using namespace GeneticLibrary::Population;
	using namespace GeneticLibrary::Population::Chromosome;
	using namespace GeneticLibrary::Operation::Mutation;
	using namespace GeneticLibrary::Operation::Choosing;

	class DemoChromosomeEvaluator: public FitnessEvaluator<double>
	{
	public:
		DemoChromosomeEvaluator(){};
		Fitness<double>::ptr evaluate(const BaseChromosome<double>::ptr chromosome) const
		{
			MultiValueChromosome<int,double>::ptr mvc = boost::dynamic_pointer_cast<MultiValueChromosome<int,double> >(chromosome);
			if(mvc){
				//Let the fitness be the sum of all values
				return boost::shared_ptr<Fitness<double> > (new Fitness<double>(mvc->getSum()));
			}else{
				throw new std::runtime_error("Chromosome is not an Integer MultiValueChromosome with double fitness!");
			}

			boost::shared_ptr<Fitness<double> > ptr(new Fitness<double>(1));
			return ptr;
		}
	};


BOOST_AUTO_TEST_CASE( basicMutation )
{
	/*100% Chance of mutation
	 * Tests if Chromosomes are actually mutated
	 *
	 * Test for Uniform and NonUniform Mutation
	 */


	DemoChromosomeEvaluator::ptr evaluator(new DemoChromosomeEvaluator());
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
		BaseMutationOperation<double> *mutationOperation_Uniform = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

		BaseChromosome<double>::ptr _newChromosome = chromosomeFactory->createChromosome(true);
		GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
		GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set resultSet_NonUniform;
		GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set resultSet_Uniform;

		inputSet.push_back(_newChromosome);
		resultSet_NonUniform.push_back(_newChromosome);
		resultSet_Uniform.push_back(_newChromosome);

		resultSet_NonUniform = mutationOperation_NonUniform->doMutate(inputSet,manager);
		resultSet_Uniform = mutationOperation_Uniform->doMutate(inputSet,manager);

		BOOST_TEST_MESSAGE ("Checking at amount of Mutation = "<< i);
		BOOST_CHECK(inputSet != resultSet_NonUniform);
		BOOST_CHECK(inputSet != resultSet_Uniform);
	}

	BOOST_TEST_MESSAGE ("");
	BOOST_TEST_MESSAGE ("Checking Mutation at 0% probability");
	for (double i = 0; i <= 1; i = i + 0.1)
		{
			MutationSettings* mutationSettings = new MutationSettings(0,i,0);
			ChooseRandom<int,double> *mutationChoosingOperation = new ChooseRandom<int,double>(mutationSettings);
			BaseMutationOperation<double> *mutationOperation_NonUniform = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);
			BaseMutationOperation<double> *mutationOperation_Uniform = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

			BaseChromosome<double>::ptr _newChromosome = chromosomeFactory->createChromosome(true);
			GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
			GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set resultSet_NonUniform;
			GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set resultSet_Uniform;

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


BOOST_AUTO_TEST_CASE( Mutation_Propability )
{
	/*
	 * Testing Mutation Propability for 10000 Testcases (at 50% Chance of mutation)
	 * Checking UNIFOM and NONUNIFORM mutation
	 *
	 * Test Fails if less than 4850 and more than 5150 Chromosomes are Mutated
	 */


	DemoChromosomeEvaluator::ptr evaluator(new DemoChromosomeEvaluator());
	PopulationSettings *populationSettings = new PopulationSettings(50);
	ContinousMultiValueBuilderSettings<int,double> *builderSettings = new ContinousMultiValueBuilderSettings<int,double>(evaluator,10,130,0,true,20,5);
	ContinousMultiIntValueChromosomeFactory<double> *chromosomeFactory = new ContinousMultiIntValueChromosomeFactory<double>(builderSettings);

	BaseManager<double> manager(populationSettings, chromosomeFactory);


	for (double propability = -0.2; propability <= 1.2; propability = propability + 0.1){

		BOOST_TEST_MESSAGE("");


		MutationSettings* mutationSettings = new MutationSettings(propability,1,5);
		ChooseRandom<int,double> *mutationChoosingOperation = new ChooseRandom<int,double>(mutationSettings);
		BaseMutationOperation<double> *mutationOperation_NonUniform = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);
		BaseMutationOperation<double> *mutationOperation_Uniform = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

		BaseChromosome<double>::ptr _newChromosome = chromosomeFactory->createChromosome(true);
		GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
		GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set resultSet_NonUniform[10000];
		GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set resultSet_Uniform[10000];

		inputSet.push_back(_newChromosome);

		int mutationCounter_NonUniform = 0;
		int mutationCounter_Uniform = 0;
		for (int i = 0; i < 10000; i++){
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
			BOOST_CHECK(mutationCounter_NonUniform > (10000*propability-150));
			BOOST_CHECK(mutationCounter_NonUniform < (10000*propability+150));

			BOOST_TEST_MESSAGE("Non-Uniform-Mutation porpability: "<< propability);
			BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_NonUniform);

			//Checking for UNIFORM Mutation
			BOOST_CHECK(mutationCounter_Uniform > (10000*propability-150));
			BOOST_CHECK(mutationCounter_Uniform < (10000*propability+150));

			BOOST_TEST_MESSAGE("Uniform-Mutation porpability: "<< propability);
			BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter_Uniform);
		}
	}
}

BOOST_AUTO_TEST_CASE ( points_of_mutation )
{
	/*
	 * Checking if as many points are mutated as set in Mutation settings
	 */

	DemoChromosomeEvaluator::ptr evaluator(new DemoChromosomeEvaluator());
	PopulationSettings *populationSettings = new PopulationSettings(50);
	ContinousMultiValueBuilderSettings<int,double> *builderSettings = new ContinousMultiValueBuilderSettings<int,double>(evaluator,10,130,0,true,20,5);
	ContinousMultiIntValueChromosomeFactory<double> *chromosomeFactory = new ContinousMultiIntValueChromosomeFactory<double>(builderSettings);

	BaseManager<double> manager(populationSettings, chromosomeFactory);

	MutationSettings* mutationSettings = new MutationSettings(1,1,5);
	ChooseRandom<int,double> *mutationChoosingOperation = new ChooseRandom<int,double>(mutationSettings);
	BaseMutationOperation<double> *mutationOperation_NonUniform = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);
	BaseMutationOperation<double> *mutationOperation_Uniform = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

	BaseChromosome<double>::ptr _newChromosome = chromosomeFactory->createChromosome(true);
	GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
	GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set resultSet_NonUniform;
	GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set resultSet_Uniform;

	inputSet.push_back(_newChromosome);
	resultSet_NonUniform.push_back(_newChromosome);
	resultSet_Uniform.push_back(_newChromosome);

	resultSet_NonUniform = mutationOperation_NonUniform->doMutate(inputSet,manager);
	resultSet_Uniform = mutationOperation_Uniform->doMutate(inputSet,manager);

	//TODO Lukas Finish mutation
	//resultSet_NonUniform.
	BOOST_CHECK(inputSet != resultSet_NonUniform);
	BOOST_CHECK(inputSet != resultSet_Uniform);

}

BOOST_AUTO_TEST_SUITE_END()

