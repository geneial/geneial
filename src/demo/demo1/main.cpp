/*
 * main.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */


#include <algorithm/BaseGeneticAlgorithm.h>
#include <algorithm/criteria/MaxIterationCriterion.h>
#include <core/population/PopulationSettings.h>
#include <core/population/builder/MultiValueBuilderSettings.h>
#include <core/population/builder/MultiIntValueChromosomeFactory.h>
#include <Config.h>

using namespace GeneticLibrary::Algorithm;
using namespace GeneticLibrary::Algorithm::StoppingCriteria;
using namespace GeneticLibrary::Population;

int main(int argc, char **argv) {

	std::cout << "Running GENEIAL demo1 - Version " << GENEIAL_VERSION_MAJOR << "." << GENEIAL_VERSION_MINOR << std::endl;


	//TODO (bewo): write example demo

	PopulationSettings *populationSettings = new PopulationSettings(50);

	MultiValueBuilderSettings<int> builderSettings = MultiValueBuilderSettings<int>(50,0,130);

	BaseChromosomeFactory<double> *chromosomeFactory = new MultiIntValueChromosomeFactory<double>(builderSettings);

	StoppingCriteria::BaseStoppingCriterion<double> *stoppingCriterion = new MaxIterationCriterion<double>(100);

	BaseGeneticAlgorithm<double> algorithm = BaseGeneticAlgorithm<double>(
			populationSettings,
			chromosomeFactory,
			stoppingCriterion
	);

	algorithm.solve();
}
