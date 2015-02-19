/*
 * mutation_test.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: lukas
 */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE operations/mutation
#include <boost/test/unit_test.hpp>

#include <geneial/core/operations/mutation/BaseMutationOperation.h>

#include <geneial/core/operations/mutation/NonUniformMutationOperation.h>
#include <geneial/core/operations/mutation/UniformMutationOperation.h>


#include <geneial/core/fitness/FitnessEvaluator.h>
#include <geneial/core/fitness/Fitness.h>

BOOST_AUTO_TEST_SUITE( UniformMutationOperation )

	using namespace GeneticLibrary;


BOOST_AUTO_TEST_CASE( basicMutation )
{

	/*
	using namespace GeneticLibrary::Population::Chromosome;
	using namespace GeneticLibrary::Operation::Mutation;
	using namespace GeneticLibrary;
	//TODO (Lukas) clean up this mess
	DemoChromosomeEvaluator::ptr evaluator(new DemoChromosomeEvaluator());
	typename BaseChromosome<double>::ptr _newChromosome = GeneticLibrary::Population::Chromosome::MultiIntValueChromosomeFactory<double>::createChromosome();
	typename GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set inputSet;
	typename GeneticLibrary::Operation::Mutation::BaseMutationOperation<double>::mutation_result_set resultSet;

	resultSet.push_back(_newChromosome);

	GeneticLibrary::Population::Chromosome::MultiValueBuilderSettings<int,double> *builderSettings = new GeneticLibrary::Population::Chromosome::MultiValueBuilderSettings<int,double>(evaluator,10,0,130);
	MutationSettings* mutationSettings = new MutationSettings(0.1,0.1,0);
	MultiIntValueChromosomeFactory<double> *chromosomeFactory = new MultiIntValueChromosomeFactory<double>(builderSettings);
	BaseMutationOperation<double> *mutationOperation = new UniformMutationOperation<int,double>(mutationSettings, builderSettings, chromosomeFactory);
	resultSet = mutationOperation->doMutate(inputSet);


	BOOST_CHECK(inputSet != resultSet);
	*/
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( NonUniformMutationOperation )

BOOST_AUTO_TEST_SUITE_END()
