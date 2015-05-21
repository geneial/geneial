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
        std::unique_ptr<Fitness<double>> ptr(new Fitness<double>);
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

    auto algorithm = SteadyStateAlgorithm<double>::Builder().
            setChromosomeFactory(std::make_shared<ContinousMultiValueChromosomeFactory<int,double>>(chromosomeFactory)).
            build();

    algorithm->getPopulationSettings().setMaxChromosomes(100);

    algorithm->setExecutionManager(std::move(std::unique_ptr<ThreadedExecutionManager>(new ThreadedExecutionManager(1))));

    algorithm->solve();

    std::cout << *algorithm->getHighestFitnessChromosome() << std::endl;

    std::cout << "end." << std::endl;

}
