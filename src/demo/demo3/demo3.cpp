#include <limits>
#include <stdio.h>
#include <stdexcept>
#include <cassert>

#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <geneial/algorithm/SteadyStateAlgorithm.h>

#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>

#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>

#include <geneial/core/operations/mutation/SmoothPeakMutationOperation.h>
#include <geneial/core/operations/crossover/MultiValueChromosomeBlendingCrossover.h>
#include <geneial/core/operations/crossover/SmoothedMultiValueChromosomeNPointCrossover.h>

#include <geneial/algorithm/criteria/CombinedCriterion.h>
#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>
#include <geneial/algorithm/criteria/NegationDecorator.h>
#include <geneial/algorithm/criteria/FixPointCriterion.h>


#include <geneial/algorithm/observer/BestChromosomeObserver.h>

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

double myTargetFunc1(double x)
{
    return std::sin(x) * std::abs(std::sin(M_PI / 2 + x * x / 2)) * 30 + 50;
}
/*
double myTargetFunc2(double x)
{
    //Some Lagrange poly + Manual Tweakage
    x = 0.55 * x + 0.3;
    const double result = (84211 * std::pow(x, 6) - 4829676 * std::pow(x, 5) + 104637796 * std::pow(x, 4)
            - 1070636286 * std::pow(x, 3) + 5268146305 * std::pow(x, 2) - 11346283350 * x + 10783521000) / 37346400
            + (2.10 * 0.001 * x * x * x * (x / 2));
    return result;
}

double myTargetFunc3(double x)
{
    return 200/x;
}
*/
void plot(const MultiValueChromosome<int, double> &chromosomeToPrint)
{
    const auto & container = chromosomeToPrint.getContainer();

    const double xmax = 30;
    const double xstep = 0.25;

    const double ymax = 180;
    const double ystep = 20;

    for (double y = ymax; y >= 0; y -= ystep)
    {
        for (double x = 0; x < xmax; x += xstep)
        {
            char out = ' ';

            const double result = myTargetFunc1(x);
            if (y <= result && result < y + ystep)
            {
                out = '+';
            }

            const double result2 = container[x];
            if (y <= result2 && result2 < y + ystep)
            {
                if (out == '+')
                {
                    out = 'X';
                }
                else
                {
                    out = '-';
                }
            }
            std::cout << out;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Age:" << chromosomeToPrint.getAge();
    std::cout << " Fitness:" << chromosomeToPrint.getFitness().get();
    std::cout << std::endl;

    std::cout.width(15);
    std::cout.width(1);
    std::cout << "|";
    std::cout.width(15);
    std::cout << std::left << " X ";
    std::cout.width(1);
    std::cout << "|";
    std::cout.width(15);
    std::cout << std::left << " Y ";
    std::cout.width(1);
    std::cout << "|";
    std::cout.width(15);
    std::cout << std::left << " Target ";
    std::cout.width(1);
    std::cout << "|";
    std::cout.width(15);
    std::cout << std::left << " |y1-y2| " << "|";
    std::cout << std::endl;
    for (int i = 0; i < 30; i++)
    {

        std::cout.width(15);
        std::cout.width(1);
        std::cout << "|";
        std::cout.width(15);
        std::cout << std::right << i + 1;
        std::cout.precision(6);
        std::cout.setf(std::ios::fixed, std::ios::floatfield);
        std::cout.width(1);
        std::cout << "|";
        std::cout.width(15);
        std::cout << std::right << container[i];
        std::cout.width(1);
        std::cout << "|";
        std::cout.width(15);
        std::cout << std::right << myTargetFunc1(i);
        std::cout.width(1);
        std::cout << "|";
        std::cout.width(15);
        std::cout << std::right << std::abs(myTargetFunc1(i) - container[i]) << "|";
        std::cout << std::endl;
    }
}

class DemoChromosomeEvaluator: public FitnessEvaluator<double>
{
public:
    std::unique_ptr<Fitness<double>> evaluate(const BaseChromosome<double>& chromosome) const
    {
        try
        {
            double fitness = 1;
            const auto& mvc = dynamic_cast<const MultiValueChromosome<int, double>&>(chromosome);
            int i = 0;
            for (auto value : mvc.getContainer())
            {
                fitness += std::abs(value - myTargetFunc1(i));
                i++;
            }
            return std::move(std::unique_ptr<Fitness<double> >(new Fitness<double>(1 / fitness)));
        } catch (std::bad_cast&)
        {
            throw new std::runtime_error("Chromosome is not an Integer MultiValueChromosome with double fitness!");
        }
        std::unique_ptr<Fitness<double> > ptr(new Fitness<double>(std::numeric_limits<double>::signaling_NaN()));
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

class DemoObserver: public BestChromosomeObserver<double>
{
public:
    virtual void updateNewBestChromosome(geneial::population::management::BaseManager<double> &manager)
    {
        auto best = std::dynamic_pointer_cast< MultiValueChromosome<int, double> > (manager.getHighestFitnessChromosome());
        printClearScreen();
        plot(*best);
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
    ContinousMultiValueChromosomeFactory<int,double>::Builder factoryBuilder(evaluator);
    factoryBuilder.getSettings().setNum(30);
    factoryBuilder.getSettings().setRandomMin(-250);
    factoryBuilder.getSettings().setRandomMax(250);
    factoryBuilder.getSettings().setHasStart(false);
    factoryBuilder.getSettings().setEps(100);

    auto factory  = std::dynamic_pointer_cast<ContinousMultiValueChromosomeFactory<int, double>>(factoryBuilder.create());
    algorithmBuilder.setChromosomeFactory(factory);

    //Mutation:
    SmoothPeakMutationOperation<int,double>::Builder mutationBuilder(factory);

    auto choosing = ChooseRandom<double>::Builder().setProbability(0.4).create();
    mutationBuilder.setMaxElevation(20);
    mutationBuilder.setMaxLeftEps(5);
    mutationBuilder.setMaxRightEps(5);

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
    auto crossoverBuilder = SmoothedMultiValueChromosomeNPointCrossover<int, double>::Builder(factory);
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
            std::move(std::make_shared<MaxGenerationCriterion<double>>(1000000)));

    //stoppingCriterion->add(CombinedCriterion<double>::OR,
     //      std::move(std::make_shared<FixPointCriterion<double>>(0, 50000, 50000)));


    algorithmBuilder.setStoppingCriterion(stoppingCriterion);

    auto algorithm = algorithmBuilder.create();
    algorithm->getPopulationSettings().setMaxChromosomes(100);

    algorithm->registerObserver(std::make_shared<DemoObserver>());

    algorithm->solve();


    printClearScreen();
    auto mvc = std::dynamic_pointer_cast<MultiValueChromosome<int, double> >(
            algorithm->getHighestFitnessChromosome());
    plot (*mvc);
    std::cout << "ended after " << algorithm->getPopulation().getAge() << " generations" << std::endl;


}
