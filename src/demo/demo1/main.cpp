/*
 * main.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#include "boost/shared_ptr.hpp"
#include <geneial/algorithm/BaseGeneticAlgorithm.h>
#include <geneial/algorithm/criteria/MaxIterationCriterion.h>
#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/fitness/FitnessEvaluator.h>
#include <geneial/core/population/PopulationSettings.h>
#include <geneial/core/population/builder/MultiValueBuilderSettings.h>
#include <geneial/core/population/builder/MultiIntValueChromosomeFactory.h>
#include <geneial/core/operations/selection/FitnessProportionalSelection.h>
#include <geneial/core/operations/selection/FitnessProportionalSelectionSettings.h>
#include <geneial/config.h>

#include <stdexcept>

using namespace GeneticLibrary;
using namespace GeneticLibrary::Algorithm;
using namespace GeneticLibrary::Algorithm::StoppingCriteria;

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Chromosome;

using namespace GeneticLibrary::Operation::Selection;


class DemoChromosomeEvaluator: public FitnessEvaluator<double>{
public:
	DemoChromosomeEvaluator(){};
	Fitness<double>::ptr evaluate(const BaseChromosome<double>::ptr chromosome) const{

		MultiValueChromosome<int,double>::ptr mvc = boost::dynamic_pointer_cast<MultiValueChromosome<int,double> >(chromosome);
		if(mvc){
			//Let the fitness be the sum of all values
			return boost::shared_ptr<Fitness<double> > (new Fitness<double>(mvc->getSum()));
		}else{
			throw new std::runtime_error("Chromosome is not an Integer MultiValueChromosome with double fitness!");
		}
		boost::shared_ptr<Fitness<double> > ptr(new Fitness<double>(1));
		return ptr;
	};
};

int main(int argc, char **argv) {

	std::cout << "Running GENEIAL demo1 - Version " << GENEIAL_VERSION_MAJOR << "." << GENEIAL_VERSION_MINOR << std::endl;


	//TODO (bewo): write reasonable example demo

	PopulationSettings *populationSettings = new PopulationSettings(50);

	MultiValueBuilderSettings<int> builderSettings = MultiValueBuilderSettings<int>(50,0,130);

	BaseChromosomeFactory<double> *chromosomeFactory = new MultiIntValueChromosomeFactory<double>(builderSettings);

	FitnessProportionalSelectionSettings* selectionSettings = new FitnessProportionalSelectionSettings(10,2);

	BaseSelectionOperation<double> *selectionOperation = new FitnessProportionalSelection<double>(selectionSettings);

	DemoChromosomeEvaluator *evaluator = new DemoChromosomeEvaluator();

	BaseStoppingCriterion<double> *stoppingCriterion = new MaxIterationCriterion<double>(1000);

	BaseGeneticAlgorithm<double> algorithm = BaseGeneticAlgorithm<double>(
			populationSettings,
			chromosomeFactory,
			stoppingCriterion,
			evaluator,
			selectionOperation
	);

	algorithm.solve();

	//normally, this is not necessary because we're exiting here anyway,
	//but for valgrind's satisfaction, we free stuff nonetheless.
	delete populationSettings;
	delete evaluator;
	delete chromosomeFactory;
	delete selectionOperation;
	delete selectionSettings;
	delete stoppingCriterion;
}
