#include <geneial/algorithm/SteadyStateAlgorithm.h>

#include <geneial/algorithm/criteria/CombinedCriterion.h>
#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>
#include <geneial/algorithm/criteria/NegationDecorator.h>
#include <geneial/algorithm/criteria/FitnessValueReachedCriterion.h>

#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>

#include <geneial/core/operations/replacement/ReplaceWorstOperation.h>
#include <geneial/core/operations/replacement/ReplaceRandomOperation.h>

#include <geneial/core/operations/mutation/UniformMutationOperation.h>

#include <geneial/core/operations/selection/RouletteWheelSelection.h>

#include <geneial/core/operations/coupling/RandomCouplingOperation.h>


#include <geneial/core/operations/choosing/ChooseRandom.h>

#include <geneial/core/fitness/FitnessEvaluator.h>

#include <geneial/utility/ThreadedExecutionManager.h>

#include <geneial/algorithm/observer/BestChromosomeObserver.h>



#include <limits>
#include <stdio.h>
#include <stdexcept>
#include <cassert>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>

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

using namespace geneial::utility;

const char targetFigure[] =
//0123456789|123456789|123456789|123456789
 "              OOOOOOOOOOO               " //01
 "          OOOOOOOOOOOOOOOOOOO           " //02
 "       OOOOOO**OOOOOOOOO**OOOOOO        " //03
 "     OOOOOO  -   OOOOO   -  OOOOOO      " //04
 "   OOOOOOOO #    OOOOO  #   OOOOOOOO    " //05
 "  OOOOOOOOOO    OOOOOOO    OOOOOOOOOO   " //06
 " OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  " //07
 " OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  " //08
 " OOOO  OOOOOOOOOOOOOOOOOOOOOOOOO  OOOO  " //09
 "  OOOO  OOOOOOOOOOOOOOOOOOOOOOO  OOOO   " //10
 "   OOOO   OOOOOOOOOOOOOOOOOOOO  OOOO    " //11
 "     OOOOO   OOOOOOOOOOOOOOO   OOOO     " //12
 "       OOOOOO   OOOOOOOOO   OOOOOO      " //13
 "         OOOOOO         OOOOOO          " //14
 "              OOOOOOOOOOOO              " //15
 "                                        " //16
 "                 =======                " //16
 "                 GENEIAL                " //17
 "                 =======                ";//16
const int lineBreakAfter = 40;
const int charsPerFigure = sizeof(targetFigure) - 1;

class DemoChromosomeEvaluator: public FitnessEvaluator<double>
{
public:
    std::unique_ptr<Fitness<double>> evaluate(const BaseChromosome<double>& chromosome) const
    {
        try
        {
            const MultiValueChromosome<int, double>& mvc =
                    dynamic_cast<const MultiValueChromosome<int, double>&>(chromosome);

            double fitness = 0;
            assert(charsPerFigure == mvc.getContainer().size());
            for (int i = 0; i < charsPerFigure; i++)
            {
                if (targetFigure[i] == static_cast<char>(mvc.getContainer()[i]))
                {
                    ++fitness;
                }
            }
            return std::unique_ptr<Fitness<double>>(new Fitness<double>(fitness));
        } catch (std::bad_cast&)
        {
            throw new std::runtime_error("Chromosome is not an Integer MultiValueChromosome with double fitness!");
        }
        std::unique_ptr<Fitness<double>> ptr(new Fitness<double>);
		return ptr;
	}
};

void inline printClearScreen()
{
#ifdef WINDOWS
	if(!
		std::system ( "CLS" ))
	{
	assert("Unable to clear Screen");
	}
#else
	// Assume POSIX
	if (!std::system("clear"))
	{
	assert("Unable to clear Screen");
	}
#endif
}

void printChromosome(const MultiValueChromosome<int, double> &chromosomeToPrint)
{
	std::cout << std::endl;
	int it = 0;
	assert(chromosomeToPrint.getContainer().size() == charsPerFigure);
	while (it < charsPerFigure)
	{
	std::cout << (char) chromosomeToPrint.getContainer()[it];
	if (it != 0 && 0 == (it % lineBreakAfter))
	{
		std::cout << std::endl;
	}
	++it;
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Age:" << chromosomeToPrint.getAge();
	std::cout << " Fitness:" << chromosomeToPrint.getFitness().get();
	std::cout << std::endl;
}

bool running = true;
std::condition_variable cv;
std::mutex cv_m;
MultiValueChromosome<int, double>::ptr best;

void displayOnUpdate()
{
	MultiValueChromosome<int, double>::ptr _best;

	while (running)
	{
	std::unique_lock<std::mutex> l(cv_m);
	cv.wait(l);
	_best = best;
	printClearScreen();
	printChromosome(*_best);
	}

	std::cerr << "...finished waiting. i == 1\n";
}

class DemoObserver: public BestChromosomeObserver<double>
{
public:
	virtual void updateNewBestChromosome(geneial::population::management::BaseManager<double> &manager)
	{
	try
	{
		best = std::dynamic_pointer_cast< MultiValueChromosome<int, double> > (manager.getHighestFitnessChromosome());
		cv.notify_all();
	} catch (std::bad_cast &)
	{
		throw new std::runtime_error("Chromosome is not an Integer MultiValueChromosome with double fitness!");
	}
	}

};

int main(int argc, char **argv)
{


	std::cout
		<< "Running GENEIAL demo2 - Version "
		<< GENEIAL_VERSION_STRING << " ("<<GENEIAL_BUILD_TYPE << ")"
		<< std::endl;

	auto evaluator = std::make_shared<DemoChromosomeEvaluator>();

	auto algorithmBuilder = SteadyStateAlgorithm<double>::Builder();

	//Factory:
	MultiValueChromosomeFactory<int, double>::Builder factoryBuilder(evaluator);
	factoryBuilder.getSettings().setNum(charsPerFigure);
	factoryBuilder.getSettings().setRandomMin(32);
	factoryBuilder.getSettings().setRandomMax(128);

	auto factory  = std::dynamic_pointer_cast<MultiValueChromosomeFactory<int, double>>(factoryBuilder.create());
	algorithmBuilder.setChromosomeFactory(factory);

	//Mutation:
	UniformMutationOperation<int,double>::Builder mutationBuilder(factory);

	auto choosing = ChooseRandom<double>::Builder().setProbability(0.4).create();
	mutationBuilder.setChoosingOperation(choosing);
	mutationBuilder.getSettings().setMinimumPointsToMutate(1);
	mutationBuilder.getSettings().setMaximumPointsToMutate(10);

	algorithmBuilder.setMutationOperation(mutationBuilder.create());

	//Selection:
	auto selectionBuilder = RouletteWheelSelection<double>::Builder();
	selectionBuilder.getSettings().setNumberOfParents(10);

	algorithmBuilder.setSelectionOperation(selectionBuilder.create());

	//Coupling:
	auto couplingBuilder = RandomCouplingOperation<double>::Builder();
	couplingBuilder.getSettings().setNumberOfOffspring(20);

	algorithmBuilder.setCouplingOperation(couplingBuilder.create());


	//Crossover:
	auto crossoverBuilder = MultiValueChromosomeNPointCrossover<int, double>::Builder(factory);
	crossoverBuilder.getCrossoverSettings().setCrossOverPoints(2);
	crossoverBuilder.getCrossoverSettings().setWidthSetting(MultiValueChromosomeNPointCrossoverSettings::RANDOM_MIN_WIDTH);
	crossoverBuilder.getCrossoverSettings().setMinWidth(3);

	algorithmBuilder.setCrossoverOperation(crossoverBuilder.create());

	//Replacement:
	auto replacementBuilder = ReplaceWorstOperation<double>::Builder();
	//auto replacementBuilder = ReplaceRandomOperation<double>::Builder();
	replacementBuilder.getSettings().setMode(BaseReplacementSettings::REPLACE_ALL_OFFSPRING);
	replacementBuilder.getSettings().setAmountElitism(20);
	//replacementBuilder.getSettings().setAmountToReplace(30);

	algorithmBuilder.setReplacementOperation(replacementBuilder.create());

	//Stopping Criteria
	auto stoppingCriterion = std::make_shared<CombinedCriterion<double>>();
	stoppingCriterion->add(CombinedCriterion<double>::INIT,
		std::make_shared<MaxGenerationCriterion<double>>(1000000));

	stoppingCriterion->add(CombinedCriterion<double>::OR,
			std::make_shared<FitnessValueReachedCriterion<double>>(charsPerFigure));

	algorithmBuilder.setStoppingCriterion(stoppingCriterion);

	auto algorithm = algorithmBuilder.create();
	algorithm->getPopulationSettings().setMaxChromosomes(100);

	auto threadproto = new ThreadedExecutionManager(4);
	threadproto->setAmountPerThread(4);
	algorithm->setExecutionManager(
			std::unique_ptr < ThreadedExecutionManager > (std::move(threadproto)));

	algorithm->registerObserver(std::make_shared<DemoObserver>());

	std::thread ui_update(displayOnUpdate);

	algorithm->solve();
	running = false;
	cv.notify_all();
	cv.notify_all();
	ui_update.join();

	printClearScreen();
	auto mvc = std::dynamic_pointer_cast<MultiValueChromosome<int, double> >(
			algorithm->getHighestFitnessChromosome());
	printChromosome (*mvc);
	std::cout << "ended after " << algorithm->getPopulation().getAge() << " generations" << std::endl;
}
