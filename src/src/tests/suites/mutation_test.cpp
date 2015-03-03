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


BOOST_AUTO_TEST_CASE( NunUniform_basicMutation )
{
	//100% Chance of mutation
	DemoChromosomeEvaluator::ptr evaluator(new DemoChromosomeEvaluator());
	PopulationSettings *populationSettings = new PopulationSettings(50);
	ContinousMultiValueBuilderSettings<int,double> *builderSettings = new ContinousMultiValueBuilderSettings<int,double>(evaluator,10,130,0,true,20,5);
	ContinousMultiIntValueChromosomeFactory<double> *chromosomeFactory = new ContinousMultiIntValueChromosomeFactory<double>(builderSettings);

	BaseManager<double> manager(populationSettings, chromosomeFactory);

	MutationSettings* mutationSettings = new MutationSettings(1,1,5);
	ChooseRandom<int,double> *mutationChoosingOperation = new ChooseRandom<int,double>(mutationSettings);
	BaseMutationOperation<double> *mutationOperation = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

	BaseChromosome<double>::ptr _newChromosome = chromosomeFactory->createChromosome(true);
	GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
	GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set resultSet;

	inputSet.push_back(_newChromosome);
	resultSet.push_back(_newChromosome);
	resultSet = mutationOperation->doMutate(inputSet,manager);

	BOOST_CHECK(inputSet != resultSet);
}

BOOST_AUTO_TEST_CASE( NonUniform_Mutation_Propability )
{
	//50% Chance of mutation
	DemoChromosomeEvaluator::ptr evaluator(new DemoChromosomeEvaluator());
	PopulationSettings *populationSettings = new PopulationSettings(50);
	ContinousMultiValueBuilderSettings<int,double> *builderSettings = new ContinousMultiValueBuilderSettings<int,double>(evaluator,10,130,0,true,20,5);
	ContinousMultiIntValueChromosomeFactory<double> *chromosomeFactory = new ContinousMultiIntValueChromosomeFactory<double>(builderSettings);

	BaseManager<double> manager(populationSettings, chromosomeFactory);

	MutationSettings* mutationSettings = new MutationSettings(0.5,1,5);
	ChooseRandom<int,double> *mutationChoosingOperation = new ChooseRandom<int,double>(mutationSettings);
	BaseMutationOperation<double> *mutationOperation = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

	BaseChromosome<double>::ptr _newChromosome = chromosomeFactory->createChromosome(true);
	GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
	GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set resultSet[10000];

	inputSet.push_back(_newChromosome);
	int mutationCounter = 0;
	for (int i = 0; i < 10000; i++){
		resultSet[i].push_back(_newChromosome);
		resultSet[i] = mutationOperation->doMutate(inputSet,manager);
		if (inputSet != resultSet[i]) mutationCounter++;
	}
	BOOST_CHECK(mutationCounter > 4850);
	BOOST_CHECK(mutationCounter < 5150);
	BOOST_TEST_MESSAGE("Mutation porpability 50%:");
	BOOST_TEST_MESSAGE("Mutated chrmomosomes (Of 10000): "<< mutationCounter);
}


BOOST_AUTO_TEST_SUITE_END()

