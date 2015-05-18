#include <geneial/algorithm/SteadyStateAlgorithm.h>
#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>
#include <geneial/algorithm/criteria/NegationDecorator.h>

#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/fitness/FitnessEvaluator.h>

#include <geneial/core/population/PopulationSettings.h>

#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>

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

#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/operations/mutation/NonUniformMutationOperation.h>

#include <geneial/core/operations/choosing/ChooseRandom.h>

#include <geneial/config.h>

#include <stdexcept>
#include <cassert>
#include <memory>
#include <chrono>
#include <thread>

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

class DemoChromosomeEvaluator: public FitnessEvaluator<double>
{
public:
    DemoChromosomeEvaluator()
    {
    }

    std::unique_ptr<Fitness<double>> evaluate(const BaseChromosome<double>& chromosome) const
    {
        try
        {
            const MultiValueChromosome<int, double>& mvc = dynamic_cast<const MultiValueChromosome<int, double>&>(chromosome);
            return std::move(std::unique_ptr<Fitness<double>>(new Fitness<double>(mvc.getSum())));
        }
        catch(std::bad_cast)
        {
            throw new std::runtime_error("Chromosome is not an Integer MultiValueChromosome with double fitness!");
        }
        std::unique_ptr<Fitness<double>> ptr(new Fitness<double>(1));
        return std::move(ptr);
    }
};

int main(int argc, char **argv)
{

    std::cout
            //<< "\x1b[0m\x1b[35;1m\x1b[41;1m"
            << "Running GENEIAL demo1 - Version "
            << GENEIAL_VERSION_MAJOR
            << "." << GENEIAL_VERSION_MINOR
            << " ("
                << GENEIAL_BUILD_TYPE << ")"
            << std::endl;

    DemoChromosomeEvaluator::ptr evaluator(new DemoChromosomeEvaluator());

    ContinousMultiValueBuilderSettings<int, double> builderSettings(evaluator, 20, 130, 0, true, 20, 5);

    ContinousMultiValueChromosomeFactory<int,double> chromosomeFactory(builderSettings);

    MutationSettings mutationSettings(0.1, 0.1, 5);

    ChooseRandom<int, double> mutationChoosingOperation(mutationSettings);

    NonUniformMutationOperation<int, double> mutationOperation(1, 0.2,
            mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

//    FitnessProportionalSelectionSettings selectionSettings(5,5);

//    FitnessProportionalSelection<double> selectionOperation(selectionSettings);



    RouletteWheelSelection<double> selectionOperation(SelectionSettings(5));


    CouplingSettings couplingSettings(20);

    RandomCouplingOperation<double> couplingOperation(couplingSettings);

    MultiValueChromosomeNPointCrossoverSettings crossoverSettings (1, MultiValueChromosomeNPointCrossoverSettings::RANDOM_WIDTH, 1);

    MultiValueChromosomeNPointCrossover<int, double> crossoverOperation(crossoverSettings, builderSettings, chromosomeFactory);

    BaseReplacementSettings replacementSettings(BaseReplacementSettings::REPLACE_ALL_OFFSPRING, 5, 2);

    ReplaceWorstOperation<double> replacementOperation(replacementSettings);

    MaxGenerationCriterion<double> stoppingCriterion(100000);

    SteadyStateAlgorithm<double> algorithm(
            std::make_shared<MaxGenerationCriterion<double>>(stoppingCriterion),
            std::make_shared<RouletteWheelSelection<double>>(selectionOperation),
            std::make_shared<RandomCouplingOperation<double>>(couplingOperation),
            std::make_shared<MultiValueChromosomeNPointCrossover<int, double>>(crossoverOperation),
            std::make_shared<ReplaceWorstOperation<double>>(replacementOperation),
            std::make_shared<NonUniformMutationOperation<int, double>>(mutationOperation),
            std::make_shared<ContinousMultiValueChromosomeFactory<int,double>>(chromosomeFactory));

    algorithm.getPopulationSettings().setMaxChromosomes(100);

    algorithm.setExecutionManager(std::move(std::unique_ptr<ThreadedExecutionManager>(new ThreadedExecutionManager(1))));

    algorithm.solve();

    std::cout << *algorithm.getHighestFitnessChromosome() << std::endl;

    std::cout << "end." << std::endl;

}
