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
#include <geneial/core/operations/mutation/UniformMutationOperation.h>

#include <geneial/core/operations/choosing/ChooseRandom.h>

#include <geneial/config.h>

#include "Sphere.h"
#include "Ackley.h"


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

class SphereEvaluator: public FitnessEvaluator<double>
{
public:
    std::unique_ptr<Fitness<double>> evaluate(const BaseChromosome<double>& chromosome) const
    {
        try
        {
            const MultiValueChromosome<double, double>& mvc = dynamic_cast<const MultiValueChromosome<double, double>&>(chromosome);

            return std::move(std::unique_ptr<Fitness<double>>(new Fitness<double>( -1 * Sphere::compute(mvc.getContainer()) )));
        }
        catch(std::bad_cast)
        {
            throw new std::runtime_error("Chromosome is not an Integer MultiValueChromosome with double fitness!");
        }
        std::unique_ptr<Fitness<double>> ptr(new Fitness<double>(1));
        return std::move(ptr);
    }
};


class AckleyEvaluator: public FitnessEvaluator<double>
{
public:
    std::unique_ptr<Fitness<double>> evaluate(const BaseChromosome<double>& chromosome) const
    {
        try
        {
            const MultiValueChromosome<double, double>& mvc = dynamic_cast<const MultiValueChromosome<double, double>&>(chromosome);

            return std::move(std::unique_ptr<Fitness<double>>(new Fitness<double>( -1 * Ackley::compute(mvc.getContainer()) )));
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
            << "GENEIAL Benchmark Suite - Version "
            << GENEIAL_VERSION_MAJOR
            << "." << GENEIAL_VERSION_MINOR
            << " ("
                << GENEIAL_BUILD_TYPE << ")"
            << std::endl;

    std::cout << "Sphere 2D" << std::endl;

    SphereEvaluator::ptr evaluator(new AckleyEvaluator());

    MultiValueBuilderSettings<double, double> builderSettings(evaluator, 2 , 500, -500 );

    MultiValueChromosomeFactory<double,double> chromosomeFactory(builderSettings);

    MutationSettings mutationSettings(0.3, 0.0, 1);

    ChooseRandom<double, double> mutationChoosingOperation(mutationSettings);

    UniformMutationOperation<double, double> mutationOperation(mutationSettings, mutationChoosingOperation,
            builderSettings, chromosomeFactory);

    RouletteWheelSelection<double> selectionOperation(SelectionSettings(5));


    CouplingSettings couplingSettings(20);

    RandomCouplingOperation<double> couplingOperation(couplingSettings);

    MultiValueChromosomeNPointCrossoverSettings crossoverSettings (1, MultiValueChromosomeNPointCrossoverSettings::EQUIDISTANT_WIDTH);

    MultiValueChromosomeNPointCrossover<double, double> crossoverOperation(crossoverSettings, builderSettings, chromosomeFactory);

    BaseReplacementSettings replacementSettings(BaseReplacementSettings::REPLACE_ALL_OFFSPRING, 5, 2);

    ReplaceWorstOperation<double> replacementOperation(replacementSettings);

    MaxGenerationCriterion<double> stoppingCriterion(1000000);

    SteadyStateAlgorithm<double> algorithm(
            stoppingCriterion, selectionOperation, couplingOperation, crossoverOperation, replacementOperation,
            mutationOperation, chromosomeFactory);

    algorithm.solve();

    std::cout << *algorithm.getHighestFitnessChromosome() << std::endl;

    std::cout << "end." << std::endl;

}
