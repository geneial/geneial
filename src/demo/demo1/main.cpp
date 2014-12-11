/*
 * main.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */


#include <algorithm/BaseGeneticAlgorithm.h>
#include <algorithm/criteria/MaxIterationCriterion.h>
#include <core/fitness/Fitness.h>
#include <core/fitness/FitnessEvaluator.h>
#include <core/population/PopulationSettings.h>
#include <core/population/builder/MultiValueBuilderSettings.h>
#include <core/population/builder/MultiIntValueChromosomeFactory.h>
#include <core/operations/selection/FitnessProportionalSelection.h>
#include <Config.h>

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
	Fitness<double>* evaluate(const BaseChromosome<double> *chromosome) const{

		const MultiValueChromosome<int,double> *mvc = dynamic_cast<const MultiValueChromosome<int,double> *>(chromosome);
		if(mvc){
			//Let the fitness be the sum of all values
			return new Fitness<double>(mvc->getSum());
		}else{
			throw new std::runtime_error("Chromosome is not an Integer Multi Value Chromosome!");
		}

		return new Fitness<double>(1);
	};
};

int main(int argc, char **argv) {

	std::cout << "Running GENEIAL demo1 - Version " << GENEIAL_VERSION_MAJOR << "." << GENEIAL_VERSION_MINOR << std::endl;


	//TODO (bewo): write reasonable example demo

	PopulationSettings *populationSettings = new PopulationSettings(50);

	MultiValueBuilderSettings<int> builderSettings = MultiValueBuilderSettings<int>(50,0,130);

	BaseChromosomeFactory<double> *chromosomeFactory = new MultiIntValueChromosomeFactory<double>(builderSettings);

	SelectionSettings* selectionSettings = new SelectionSettings(10,2);

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
}
