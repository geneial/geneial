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

//#include <geneial/core/operations/selection/FitnessProportionalSelection.h>
//#include <geneial/core/operations/selection/FitnessProportionalSelectionSettings.h>

#include <geneial/core/operations/selection/SelectionSettings.h>
#include <geneial/core/operations/selection/RouletteWheelSelection.h>

#include <geneial/core/operations/coupling/SimpleCouplingOperation.h>

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossoverSettings.h>


#include <geneial/config.h>

#include <stdexcept>

using namespace GeneticLibrary;
using namespace GeneticLibrary::Algorithm;
using namespace GeneticLibrary::Algorithm::StoppingCriteria;

using namespace GeneticLibrary::Population;
using namespace GeneticLibrary::Population::Chromosome;

using namespace GeneticLibrary::Operation::Selection;
using namespace GeneticLibrary::Operation::Coupling;
using namespace GeneticLibrary::Operation::Crossover;


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
	}
};

int main(int argc, char **argv) {

	std::cout << "Running GENEIAL demo1 - Version " << GENEIAL_VERSION_MAJOR << "." << GENEIAL_VERSION_MINOR << std::endl;


	//TODO (bewo): write reasonable example demo

	PopulationSettings *populationSettings = new PopulationSettings(200);

	MultiValueBuilderSettings<int> *builderSettings = new MultiValueBuilderSettings<int>(50,0,130);

	MultiIntValueChromosomeFactory<double> *chromosomeFactory = new MultiIntValueChromosomeFactory<double>(builderSettings);

	//FitnessProportionalSelectionSettings* selectionSettings = new FitnessProportionalSelectionSettings(20,10);
	SelectionSettings* selectionSettings = new SelectionSettings(20);

	//BaseSelectionOperation<double> *selectionOperation = new FitnessProportionalSelection<double>(selectionSettings);
	BaseSelectionOperation<double> *selectionOperation = new RouletteWheelSelection<double>(selectionSettings);

	CouplingSettings *couplingSettings = new CouplingSettings(20);

	BaseCouplingOperation<double> *couplingOperation = new SimpleCouplingOperation<double>(couplingSettings);

	MultiValueChromosomeNPointCrossoverSettings *crossoverSettings = new MultiValueChromosomeNPointCrossoverSettings(50);

	BaseCrossoverOperation<double> *crossoverOperation = new MultiValueChromosomeNPointCrossover<int,double>(crossoverSettings,builderSettings,chromosomeFactory);

	DemoChromosomeEvaluator *evaluator = new DemoChromosomeEvaluator();

	BaseStoppingCriterion<double> *stoppingCriterion = new MaxIterationCriterion<double>(10000);

	BaseGeneticAlgorithm<double> algorithm = BaseGeneticAlgorithm<double>(
			populationSettings,
			chromosomeFactory,
			stoppingCriterion,
			evaluator,
			selectionOperation,
			couplingOperation,
			crossoverOperation
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
	delete couplingOperation;
	delete crossoverOperation;
}
