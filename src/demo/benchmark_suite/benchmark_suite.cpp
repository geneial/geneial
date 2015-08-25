
#include "functions/MultiVariableOptimProblem.h"
#include "functions/Sphere.h"
#include "functions/Ackley.h"
#include "functions/F10.h"
#include "functions/F5.h"

#include <geneial/algorithm/SteadyStateAlgorithm.h>
#include <geneial/algorithm/criteria/CombinedCriterion.h>
#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>
#include <geneial/algorithm/criteria/NegationDecorator.h>
#include <geneial/algorithm/criteria/FitnessValueReachedCriterion.h>
#include <geneial/algorithm/criteria/FixPointCriterion.h>


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


#include <stdexcept>
#include <cassert>
#include <memory>
#include <chrono>
#include <thread>
#include <limits>

#include <unistd.h>

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

class GlobalMimiziationProblemEvaluator: public FitnessEvaluator<double>
{
private:
    std::shared_ptr<const MultiVariableOptimiProblem> _problemInstance;
public:

    GlobalMimiziationProblemEvaluator(std::shared_ptr<const MultiVariableOptimiProblem> problemInstance):
    _problemInstance(problemInstance)
    {
    }

    std::unique_ptr<Fitness<double>> evaluate(const BaseChromosome<double>& chromosome) const
    {
        try
        {
            const MultiValueChromosome<double, double>& mvc =
                    dynamic_cast<const MultiValueChromosome<double, double>&>(chromosome);
            return 
                    std::unique_ptr<Fitness<double>>(
                            new Fitness<double>(-1 * _problemInstance->compute(mvc.getContainer())));
        } catch (std::bad_cast)
        {
            throw new std::runtime_error("Chromosome is not an Integer MultiValueChromosome with double fitness!");
        }
        std::unique_ptr<Fitness<double>> ptr(new Fitness<double>(std::numeric_limits<double>::signaling_NaN()));
        return ptr;
    }
};

int main(int argc, char **argv)
{

    std::cout
            //<< "\x1b[0m\x1b[35;1m\x1b[41;1m"
            << "GENEIAL Benchmark Suite - Version " << GENEIAL_VERSION_MAJOR << "." << GENEIAL_VERSION_MINOR << " ("
            << GENEIAL_BUILD_TYPE << ")" << std::endl;

    std::vector <
    std::pair< unsigned int,std::shared_ptr<MultiVariableOptimiProblem>>> problems {
        {2,std::make_shared<Ackley> ()},
        {2,std::make_shared<Sphere> ()},
        {2,std::make_shared<F10> ()},
        {2,std::make_shared<F5> ()}
    };

    for (const auto & optimProblem : problems)
    {

        auto evaluator = std::make_shared<GlobalMimiziationProblemEvaluator>(optimProblem.second);

            auto algorithmBuilder = SteadyStateAlgorithm<double>::Builder();

            //Factory:
            MultiValueChromosomeFactory<double, double>::Builder factoryBuilder(evaluator);
            factoryBuilder.getSettings().setNum(optimProblem.first);
            factoryBuilder.getSettings().setRandomMin(-1000);
            factoryBuilder.getSettings().setRandomMax( 1000);

            auto factory  = std::dynamic_pointer_cast<MultiValueChromosomeFactory<double, double>>(factoryBuilder.create());
            algorithmBuilder.setChromosomeFactory(factory);

            //Mutation:
            UniformMutationOperation<double,double>::Builder mutationBuilder(factory);

            auto choosing = ChooseRandom<double>::Builder().setProbability(0.1).create();
            mutationBuilder.setChoosingOperation(choosing);
            mutationBuilder.getSettings().setMinimumPointsToMutate(1);
            mutationBuilder.getSettings().setMaximumPointsToMutate(optimProblem.first);

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
            auto crossoverBuilder = MultiValueChromosomeNPointCrossover<double, double>::Builder(factory);
            crossoverBuilder.getCrossoverSettings().setCrossOverPoints(1);
            crossoverBuilder.getCrossoverSettings().setWidthSetting(MultiValueChromosomeNPointCrossoverSettings::RANDOM_MIN_WIDTH);
            //crossoverBuilder.getCrossoverSettings().setMinWidth(3);

            algorithmBuilder.setCrossoverOperation(crossoverBuilder.create());

            //Replacement:
            auto replacementBuilder = ReplaceRandomOperation<double>::Builder();
            replacementBuilder.getSettings().setMode(BaseReplacementSettings::REPLACE_ALL_OFFSPRING);
            replacementBuilder.getSettings().setAmountElitism(20);
            //replacementBuilder.getSettings().setAmountToReplace(30);

            algorithmBuilder.setReplacementOperation(replacementBuilder.create());

            //Stopping Criteria
            auto stoppingCriterion = std::make_shared<CombinedCriterion<double>>();
            stoppingCriterion->add(CombinedCriterion<double>::INIT,
                    std::make_shared<MaxGenerationCriterion<double>>(1000000));

            algorithmBuilder.setStoppingCriterion(stoppingCriterion);

            auto algorithm = algorithmBuilder.create();
            algorithm->getPopulationSettings().setMaxChromosomes(100);


            algorithm->solve();
            auto mvc = std::dynamic_pointer_cast<MultiValueChromosome<double, double> >(
                    algorithm->getHighestFitnessChromosome());
//            std::cout << std::endl;
            std::cout << optimProblem.second->getName() << std::endl;
            std::cout << "ended after " << algorithm->getPopulation().getAge() << " generations" << std::endl;
            std::cout <<"Best chromosome" << std::endl;
            std::cout << *mvc << std::endl << std::endl;
            std::cout << "Minimum:"<<std::endl;
            for(const auto min : optimProblem.second->getMinima(optimProblem.first))
            {
                std::cout << min << ",";
            }
    }

}
