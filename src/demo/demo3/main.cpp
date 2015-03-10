#include <limits>
#include <stdio.h>
#include <stdexcept>
#include <cassert>

#include <boost/shared_ptr.hpp>

#include <geneial/algorithm/BaseGeneticAlgorithm.h>

#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/fitness/FitnessEvaluator.h>
#include <geneial/core/population/PopulationSettings.h>

#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>
#include <geneial/core/population/builder/ContinousMultiIntValueChromosomeFactory.h>

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
#include <geneial/core/operations/mutation/UniformMutationOperation.h>

#include <geneial/core/operations/choosing/ChooseRandom.h>

#include <geneial/core/fitness/MultithreadedFitnessProcessingStrategy.h>

#include <geneial/algorithm/observer/BestChromosomeObserver.h>

#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>
#include <geneial/algorithm/criteria/CombinedCriterion.h>
#include <geneial/algorithm/criteria/FixPointCriterion.h>

#include <geneial/config.h>
#include <cmath>


using namespace geneial;

using namespace geneial::algorithm;
using namespace geneial::algorithm::stopping_criteria;

using namespace geneial::population;
using namespace geneial::population::chromosome;

using namespace geneial::operation::selection;
using namespace geneial::operation::coupling;
using namespace geneial::operation::crossover;
using namespace geneial::operation::replacement;
using namespace geneial::operation::mutation;
using namespace geneial::operation::choosing;

double myTargetFunc(double x){
	//Some Lagrange poly + Manual Tweakage
	x = 0.55*x + 0.3;
	const double result =
			(	84211			* std::pow(x,6)
				-4829676		* std::pow(x,5)
				+104637796		* std::pow(x,4)
				-1070636286 	* std::pow(x,3)
				+5268146305 	* std::pow(x,2)
				-11346283350	* x
				+10783521000 ) /37346400 + (2.10*0.001*x*x*x*(x/2));
	return result;
}

void plot(MultiValueChromosome<int,double>::ptr chromosomeToPrint){
	MultiValueChromosome<int,double>::value_container container = chromosomeToPrint->getContainer();

	const double xmax = 30;
	const double xstep = 0.25;

	const double ymax = 180;
	const double ystep = 20;

	for(double y = ymax; y >= 0; y-=ystep)
	{
		for(double x = 0; x < xmax ; x += xstep)
		{
			char out = ' ';

			const double result = myTargetFunc(x);
			if( y <= result && result < y+ystep)
			{
				out = '+';
			}

			const double result2 = container[x];
			if( y <= result2 && result2 < y+ystep)
			{
				if(out == '+')
				{
					out = 'X';
				}else{
					out = '-';
				}
			}
				std::cout << out;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Age:" << chromosomeToPrint->getAge();
	std::cout << " Fitness:" << chromosomeToPrint->getFitness()->get();
	std::cout << std::endl;


	std::cout.width(15);
	std::cout.width(1); std::cout << "|"; std::cout.width(15); std::cout << std::left << " X ";
	std::cout.width(1); std::cout << "|"; std::cout.width(15); std::cout << std::left << " Y ";
	std::cout.width(1); std::cout << "|"; std::cout.width(15); std::cout << std::left << " Target ";
	std::cout.width(1); std::cout << "|"; std::cout.width(15); std::cout << std::left << " |y1-y2| " << "|";
	std::cout << std::endl;
	for(int i = 0; i< 30;i++){

		std::cout.width(15);
		std::cout.width(1); std::cout << "|"; std::cout.width(15); std::cout << std::right <<  i+1;
		std::cout.precision(6);
		std::cout.setf( std::ios::fixed, std:: ios::floatfield );
		std::cout.width(1); std::cout << "|"; std::cout.width(15); std::cout << std::right <<  container[i];
		std::cout.width(1); std::cout << "|"; std::cout.width(15); std::cout << std::right <<  myTargetFunc(i);
		std::cout.width(1); std::cout << "|"; std::cout.width(15); std::cout << std::right << std::abs(myTargetFunc(i)-container[i]) << "|";
		std::cout << std::endl;
	}
}


class DemoChromosomeEvaluator: public FitnessEvaluator<double>
{
public:
	DemoChromosomeEvaluator(){};
	Fitness<double>::ptr evaluate(const BaseChromosome<double>::ptr chromosome) const
	{
		double fitness = 1;
		MultiValueChromosome<int,double>::ptr mvc = boost::dynamic_pointer_cast<MultiValueChromosome<int,double> >(chromosome);
		if(mvc)
		{
			MultiValueChromosome<int,double>::value_container container = mvc->getContainer();

			int i = 0;
			for(MultiValueChromosome<int,double>::value_container::const_iterator it = container.begin();it != container.end();++it)
			{
				fitness += std::abs(*it - myTargetFunc(i));
				i++;
			}
			return boost::shared_ptr<Fitness<double> > (new Fitness<double>(1/fitness));
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


class DemoObserver : public BestChromosomeObserver<double>
{
public:
	DemoObserver(){};
	void updateNewBestChromosome(BaseManager<double> &manager)
	{
		MultiValueChromosome<int,double>::ptr mvc = boost::dynamic_pointer_cast<MultiValueChromosome<int,double> >(manager.getHighestFitnessChromosome());
		printClearScreen();
		plot(mvc);
	}



};

int main(int argc, char **argv) {
	std::cout << "Running GENEIAL demo2 - Version " << GENEIAL_VERSION_MAJOR << "." << GENEIAL_VERSION_MINOR << "("<< GENEIAL_BUILD_TYPE << ")"<< std::endl;

	DemoChromosomeEvaluator::ptr evaluator(new DemoChromosomeEvaluator());


	PopulationSettings *populationSettings = new PopulationSettings(1000);

	ContinousMultiValueBuilderSettings<int,double> *builderSettings = new ContinousMultiValueBuilderSettings<int,double>(evaluator,30,210,-100,false,0,10);

	ContinousMultiIntValueChromosomeFactory<double> *chromosomeFactory = new ContinousMultiIntValueChromosomeFactory<double>(builderSettings);


	MutationSettings* mutationSettings = new MutationSettings(0.4,0.0,1);

	ChooseRandom<int,double> *mutationChoosingOperation = new ChooseRandom<int,double>(mutationSettings);
	BaseMutationOperation<double> *mutationOperation = new UniformMutationOperation<int,double>(mutationSettings,mutationChoosingOperation,builderSettings,chromosomeFactory);

	//FitnessProportionalSelectionSettings* selectionSettings = new FitnessProportionalSelectionSettings(20,10);
	SelectionSettings* selectionSettings = new SelectionSettings(20);

	//BaseSelectionOperation<double> *selectionOperation = new FitnessProportionalSelection<double>(selectionSettings);
	BaseSelectionOperation<double> *selectionOperation = new RouletteWheelSelection<double>(selectionSettings);
	//BaseSelectionOperation<double> *selectionOperation = new UniformRandomSelection<double>(selectionSettings);

	CouplingSettings *couplingSettings = new CouplingSettings(20);

	//BaseCouplingOperation<double> *couplingOperation = new SimpleCouplingOperation<double>(couplingSettings);
	BaseCouplingOperation<double> *couplingOperation = new RandomCouplingOperation<double>(couplingSettings);

	MultiValueChromosomeNPointCrossoverSettings *crossoverSettings = new MultiValueChromosomeNPointCrossoverSettings(1,MultiValueChromosomeNPointCrossoverSettings::RANDOM_WIDTH,5);
	BaseCrossoverOperation<double> *crossoverOperation = new MultiValueChromosomeNPointCrossover<int,double>(crossoverSettings,builderSettings,chromosomeFactory);
	//BaseCrossoverOperation<double> *crossoverOperation = new MultiValueChromosomeAverageCrossover<int,double>(builderSettings,chromosomeFactory);

	//BaseReplacementSettings *replacementSettings = new BaseReplacementSettings(BaseReplacementSettings::replace_offspring_mode::REPLACE_FIXED_NUMBER,20);
	BaseReplacementSettings *replacementSettings = new BaseReplacementSettings(BaseReplacementSettings::REPLACE_ALL_OFFSPRING,20,0);

	ReplaceWorstOperation<double> *replacementOperation = new ReplaceWorstOperation<double>(replacementSettings);
	//ReplaceRandomOperation<double> *replacementOperation = new ReplaceRandomOperation<double>(replacementSettings);

	BaseFitnessProcessingStrategy<double> *fitnessProcessingStrategy = new MultiThreadedFitnessProcessingStrategy<double>(4);

	//BaseStoppingCriterion<double> *stoppingCriterion = new MaxGenerationCriterion<double>(100000);

	CombinedCriterion<double> combinedCriterion;
	combinedCriterion.add(CombinedCriterion<double>::AND,
				boost::shared_ptr<BaseStoppingCriterion<double> >(new MaxGenerationCriterion<double>(100000)));
	combinedCriterion.add(CombinedCriterion<double>::OR,
				boost::shared_ptr<BaseStoppingCriterion<double> >(new FixPointCriterion<double>(0,1000,1000)));


	DemoObserver printObserver;

	BaseGeneticAlgorithm<double> algorithm = BaseGeneticAlgorithm<double>(
			populationSettings,
			chromosomeFactory,
			&combinedCriterion,
			selectionOperation,
			couplingOperation,
			crossoverOperation,
			replacementOperation,
			mutationOperation,
			fitnessProcessingStrategy
	);

	algorithm.registerObserver(&printObserver);

	algorithm.solve();

	BaseChromosome<double>::ptr chromosome = algorithm.getHighestFitnessChromosome();
	MultiValueChromosome<int,double>::ptr mvc = boost::dynamic_pointer_cast<MultiValueChromosome<int,double> >(chromosome);
	printClearScreen();
	plot(mvc);
	std::cout <<  "ended after " << algorithm.getPopulation().getAge() << " generations" << std::endl;

	//normally, this is not necessary because we're exiting here anyway,
	//but for valgrind's satisfaction, we free stuff nonetheless.
	delete populationSettings;
	delete chromosomeFactory;

	delete selectionSettings;
	delete selectionOperation;

	delete fitnessProcessingStrategy;

	delete couplingSettings;
	delete couplingOperation;

	delete crossoverSettings;
	delete crossoverOperation;

	delete replacementSettings;
	delete replacementOperation;


	delete mutationSettings;
	delete mutationChoosingOperation;
	delete mutationOperation;

}
