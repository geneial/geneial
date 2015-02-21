/*
 * main.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#include <limits>
#include <stdio.h>
#include "boost/shared_ptr.hpp"

#include <geneial/algorithm/BaseGeneticAlgorithm.h>
#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>
#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/fitness/FitnessEvaluator.h>
#include <geneial/core/population/PopulationSettings.h>
#include <geneial/core/population/builder/MultiValueBuilderSettings.h>
#include <geneial/core/population/builder/MultiIntValueChromosomeFactory.h>

#include <geneial/core/operations/selection/FitnessProportionalSelection.h>
#include <geneial/core/operations/selection/FitnessProportionalSelectionSettings.h>

#include <geneial/core/operations/selection/SelectionSettings.h>
#include <geneial/core/operations/selection/RouletteWheelSelection.h>
#include <geneial/core/operations/selection/UniformRandomSelection.h>

//#include <geneial/core/operations/coupling/SimpleCouplingOperation.h>
#include <geneial/core/operations/coupling/RandomCouplingOperation.h>

#include <geneial/core/operations/replacement/BaseReplacementSettings.h>

#include <geneial/core/operations/replacement/ReplaceWorstOperation.h>
#include <geneial/core/operations/replacement/ReplaceRandomOperation.h>

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossoverSettings.h>

#include <geneial/core/operations/crossover/MultiValueChromosomeAverageCrossover.h>

#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/operations/mutation/NonUniformMutationOperation.h>

#include <geneial/core/operations/choosing/ChooseRandom.h>

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
using namespace GeneticLibrary::Operation::Replacement;
using namespace GeneticLibrary::Operation::Mutation;
using namespace GeneticLibrary::Operation::Choosing;


const char targetFigure[] =
//0123456789|123456789|123456789|123456789
 "              OOOOOOOOOOO               " //01
 "          OOOOOOOOOOOOOOOOOOO           " //02
 "       OOOOOO  OOOOOOOOO  OOOOOO        " //03
 "     OOOOOO      OOOOO      OOOOOO      " //04
 "   OOOOOOOO  #   OOOOO  #   OOOOOOOO    " //05
 "  OOOOOOOOOO    OOOOOOO    OOOOOOOOOO   " //06
 " OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  " //07
 " OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  " //08
 " OOOO  OOOOOOOOOOOOOOOOOOOOOOOOO  OOOO  " //09
 "  OOOO  OOOOOOOOOOOOOOOOOOOOOOO  OOOO   " //10
 "   OOOO   OOOOOOOOOOOOOOOOOOOO  OOOO    " //11
 "     OOOOO   OOOOOOOOOOOOOOO   OOOO     " //12
 "       OOOOOO   OOOOOOOOO   OOOOOO      " //13
 "         OOOOOO         OOOOOO          " //14
 "              OOOOOOOOOOOO              ";//15
const int lineBreakAfter = 40;
const int charsPerFigure = sizeof(targetFigure) - 1;
class DemoChromosomeEvaluator: public FitnessEvaluator<double>
{
public:
	DemoChromosomeEvaluator(){};
	Fitness<double>::ptr evaluate(const BaseChromosome<double>::ptr chromosome) const
	{
		MultiValueChromosome<int,double>::ptr mvc = boost::dynamic_pointer_cast<MultiValueChromosome<int,double> >(chromosome);
		if(mvc){
			double fitness = 0;
			assert(charsPerFigure == mvc->getContainer().size());
			for(int i = 0; i<charsPerFigure;i++)
			{
				if(targetFigure[i] == (char) mvc->getContainer()[i]){
					++fitness;
				}
			}
			//Let the fitness be the sum of all values
			return boost::shared_ptr<Fitness<double> > (new Fitness<double>(fitness));
		}else{
			throw new std::runtime_error("Chromosome is not an Integer MultiValueChromosome with double fitness!");
		}

		boost::shared_ptr<Fitness<double> > ptr(new Fitness<double>(0));
		return ptr;
	}
};

void inline printClearScreen()
{
	#ifdef WINDOWS
	  std::system ( "CLS" );
	#else
	  // Assume POSIX
	  std::system ( "clear" );
	#endif
}

void printChromosome(MultiValueChromosome<int,double>::ptr chromosomeToPrint)
{
	int it = 0;
	MultiValueChromosome<int,double>::value_container container = chromosomeToPrint->getContainer();
	assert(container.size() == charsPerFigure);
	while(it < charsPerFigure)
	{
		std::cout << (char) container[it];
		if(0 == it % lineBreakAfter)
		{
			std::cout << std::endl;
		}
		it++;
	}
}

int main(int argc, char **argv) {

	std::cout << "Running GENEIAL demo2 - Version " << GENEIAL_VERSION_MAJOR << "." << GENEIAL_VERSION_MINOR << std::endl;


	DemoChromosomeEvaluator::ptr evaluator(new DemoChromosomeEvaluator());


	PopulationSettings *populationSettings = new PopulationSettings(30);

	MultiValueBuilderSettings<int,double> *builderSettings = new MultiValueBuilderSettings<int,double>(evaluator,charsPerFigure,0,255);

	MultiIntValueChromosomeFactory<double> *chromosomeFactory = new MultiIntValueChromosomeFactory<double>(builderSettings);

	MutationSettings* mutationSettings = new MutationSettings(0.1,0.1,0);

	ChooseRandom<int,double> *mutationChoosingOperation = new ChooseRandom<int,double>(mutationSettings);

	BaseMutationOperation<double> *mutationOperation = new NonUniformMutationOperation<int,double>(1000,0.2,mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

	FitnessProportionalSelectionSettings* selectionSettings = new FitnessProportionalSelectionSettings(10,10);
	//SelectionSettings* selectionSettings = new SelectionSettings(10);

//	BaseSelectionOperation<double> *selectionOperation = new FitnessProportionalSelection<double>(selectionSettings);
	BaseSelectionOperation<double> *selectionOperation = new RouletteWheelSelection<double>(selectionSettings);
	//BaseSelectionOperation<double> *selectionOperation = new UniformRandomSelection<double>(selectionSettings);

	CouplingSettings *couplingSettings = new CouplingSettings(10);

	//BaseCouplingOperation<double> *couplingOperation = new SimpleCouplingOperation<double>(couplingSettings);
	BaseCouplingOperation<double> *couplingOperation = new RandomCouplingOperation<double>(couplingSettings);

	MultiValueChromosomeNPointCrossoverSettings *crossoverSettings = new MultiValueChromosomeNPointCrossoverSettings(1,MultiValueChromosomeNPointCrossoverSettings::RANDOM_WIDTH,1);
	BaseCrossoverOperation<double> *crossoverOperation = new MultiValueChromosomeNPointCrossover<int,double>(crossoverSettings,builderSettings,chromosomeFactory);
	//BaseCrossoverOperation<double> *crossoverOperation = new MultiValueChromosomeAverageCrossover<int,double>(builderSettings,chromosomeFactory);

	//BaseReplacementSettings *replacementSettings = new BaseReplacementSettings(BaseReplacementSettings::replace_offspring_mode::REPLACE_FIXED_NUMBER,20);
	BaseReplacementSettings *replacementSettings = new BaseReplacementSettings(BaseReplacementSettings::REPLACE_ALL_OFFSPRING,10,0);

	ReplaceWorstOperation<double> *replacementOperation = new ReplaceWorstOperation<double>(replacementSettings);
	//ReplaceRandomOperation<double> *replacementOperation = new ReplaceRandomOperation<double>(replacementSettings);


	BaseStoppingCriterion<double> *stoppingCriterion = new MaxGenerationCriterion<double>(10000000);

	BaseGeneticAlgorithm<double> algorithm = BaseGeneticAlgorithm<double>(
			populationSettings,
			chromosomeFactory,
			stoppingCriterion,
			selectionOperation,
			couplingOperation,
			crossoverOperation,
			replacementOperation,
			mutationOperation
	);

	algorithm.solve();
	std::cout << "end." << endl;

	BaseChromosome<double>::ptr chromosome = algorithm.getHighestFitnessChromosome();
	MultiValueChromosome<int,double>::ptr mvc = boost::dynamic_pointer_cast<MultiValueChromosome<int,double> >(chromosome);
	printClearScreen();
	printChromosome(mvc);

	//normally, this is not necessary because we're exiting here anyway,
	//but for valgrind's satisfaction, we free stuff nonetheless.
	delete populationSettings;
	delete chromosomeFactory;
	delete selectionOperation;
	delete selectionSettings;
	delete stoppingCriterion;
	delete couplingOperation;
	delete crossoverOperation;
	delete replacementOperation;

	delete mutationSettings;
	delete mutationChoosingOperation;
	delete mutationOperation;

}
