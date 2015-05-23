#include <geneial/algorithm/SteadyStateAlgorithm.h>

#include <geneial/algorithm/criteria/CombinedCriterion.h>
#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>
#include <geneial/algorithm/criteria/NegationDecorator.h>
#include <geneial/algorithm/criteria/FitnessValueReachedCriterion.h>

#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>

#include <geneial/core/operations/crossover/MultiValueChromosomeNPointCrossover.h>

#include <geneial/core/operations/replacement/ReplaceWorstOperation.h>

#include <geneial/core/operations/mutation/UniformMutationOperation.h>

#include <geneial/core/operations/selection/RouletteWheelSelection.h>

#include <geneial/core/operations/coupling/RandomCouplingOperation.h>


#include <geneial/core/operations/choosing/ChooseRandom.h>

#include <geneial/core/fitness/FitnessEvaluator.h>


#include <geneial/algorithm/observer/BestChromosomeObserver.h>


#include <geneial/config.h>

#include <limits>
#include <stdio.h>
#include <stdexcept>
#include <cassert>
#include <memory>

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

const char targetFigure[] =
//0123456789|123456789|123456789|123456789

        "              OOOOOOOOOOO               "//01
                "          OOOOOOOOOOOOOOOOOOO           "//02
                "       OOOOOO  OOOOOOOOO  OOOOOO        "//03
                "     OOOOOO      OOOOO      OOOOOO      "//04
                "   OOOOOOOO  #   OOOOO  #   OOOOOOOO    "//05
                "  OOOOOOOOOO    OOOOOOO    OOOOOOOOOO   "//06
                " OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  "//07
                " OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  "//08
                " OOOO  OOOOOOOOOOOOOOOOOOOOOOOOO  OOOO  "//09
                "  OOOO  OOOOOOOOOOOOOOOOOOOOOOO  OOOO   "//10
                "   OOOO   OOOOOOOOOOOOOOOOOOOO  OOOO    "//11
                "     OOOOO   OOOOOOOOOOOOOOO   OOOO     "//12
                "       OOOOOO   OOOOOOOOO   OOOOOO      "//13
                "         OOOOOO         OOOOOO          "//14
                "              OOOOOOOOOOOO              ";//15

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
            return std::move(std::unique_ptr<Fitness<double>>(new Fitness<double>(fitness)));
        } catch (std::bad_cast&)
        {
            throw new std::runtime_error("Chromosome is not an Integer MultiValueChromosome with double fitness!");
        }
        std::unique_ptr<Fitness<double>> ptr(new Fitness<double>);
        return std::move(ptr);
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
    MultiValueChromosome<int, double>::value_container container = chromosomeToPrint.getContainer();
    assert(container.size() == charsPerFigure);
    while (it < charsPerFigure)
    {
        std::cout << (char) container[it];
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

class DemoObserver: public BestChromosomeObserver<double>
{
public:
    void updateNewBestChromosome(BaseManager<double> &manager)
    {
        try
        {
            const MultiValueChromosome<int, double> & mvc = *std::dynamic_pointer_cast
                    < MultiValueChromosome<int, double> > (manager.getHighestFitnessChromosome());
            printClearScreen();
            printChromosome(mvc);
        } catch (std::bad_cast &)
        {
            throw new std::runtime_error("Chromosome is not an Integer MultiValueChromosome with double fitness!");
        }
    }

};

int main(int argc, char **argv)
{

    std::cout << "Running GENEIAL demo2 - Version " << GENEIAL_VERSION_MAJOR << "." << GENEIAL_VERSION_MINOR << " ("
            << GENEIAL_BUILD_TYPE << ")" << std::endl;

    DemoChromosomeEvaluator::ptr evaluator(new DemoChromosomeEvaluator());

    MultiValueBuilderSettings<int, double> builderSettings(evaluator, charsPerFigure, 128, 32);

    MultiValueChromosomeFactory<int, double> chromosomeFactory(builderSettings);

    MutationSettings mutationSettings(0.8, 0.0, 1);

    ChooseRandom<int, double> mutationChoosingOperation(mutationSettings);

    UniformMutationOperation<int, double> mutationOperation(mutationSettings, mutationChoosingOperation,
            builderSettings, chromosomeFactory);

    RouletteWheelSelection<double> selectionOperation(SelectionSettings(5));

    CouplingSettings couplingSettings(20);

    RandomCouplingOperation<double> couplingOperation(couplingSettings);

    MultiValueChromosomeNPointCrossoverSettings crossoverSettings(1,
            MultiValueChromosomeNPointCrossoverSettings::RANDOM_WIDTH, 3);

    MultiValueChromosomeNPointCrossover<int, double> crossoverOperation(crossoverSettings, builderSettings,
            chromosomeFactory);

    BaseReplacementSettings replacementSettings(BaseReplacementSettings::REPLACE_ALL_OFFSPRING, 20, 0);

    ReplaceWorstOperation<double> replacementOperation(replacementSettings);

    CombinedCriterion<double> stoppingCriterion;
    stoppingCriterion.add(CombinedCriterion<double>::INIT,
            std::move(std::shared_ptr<MaxGenerationCriterion<double>>(new MaxGenerationCriterion<double>(100000))));

    stoppingCriterion.add(CombinedCriterion<double>::OR,
            std::move(std::shared_ptr<FitnessValueReachedCriterion<double>>(new FitnessValueReachedCriterion<double>(600))));

    DemoObserver printObserver;


    SteadyStateAlgorithm<double> algorithm(
            std::make_shared<CombinedCriterion<double>>(stoppingCriterion),
            std::make_shared<RouletteWheelSelection<double>>(selectionOperation),
            std::make_shared<RandomCouplingOperation<double>>(couplingOperation),
            std::make_shared<MultiValueChromosomeNPointCrossover<int, double>>(crossoverOperation),
            std::make_shared<ReplaceWorstOperation<double>>(replacementOperation),
            std::make_shared<UniformMutationOperation<int, double>>(mutationOperation),
            std::make_shared<MultiValueChromosomeFactory<int, double>>(chromosomeFactory));

    algorithm.getPopulationSettings().setMaxChromosomes(60);

    algorithm.setExecutionManager(
            std::move(std::unique_ptr < ThreadedExecutionManager > (new ThreadedExecutionManager(1))));

    algorithm.registerObserver(std::make_shared<DemoObserver>(printObserver));

    algorithm.solve();

    printClearScreen();
    MultiValueChromosome<int, double>::ptr mvc = std::dynamic_pointer_cast<MultiValueChromosome<int, double> >(
            algorithm.getHighestFitnessChromosome());
    printChromosome (*mvc);
    std::cout << "ended after " << algorithm.getPopulation().getAge() << " generations" << std::endl;


}
