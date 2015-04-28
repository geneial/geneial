#include "boost/shared_ptr.hpp"

#include <geneial/algorithm/SteadyStateAlgorithm.h>
#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>
#include <geneial/algorithm/criteria/NegationDecorator.h>

#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/fitness/FitnessEvaluator.h>

#include <geneial/core/population/PopulationSettings.h>

#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>

//#include <geneial/core/operations/selection/FitnessProportionalSelection.h>
//#include <geneial/core/operations/selection/FitnessProportionalSelectionSettings.h>

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
#include <cassert>

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
    ;
    Fitness<double>::ptr evaluate(const BaseChromosome<double>::ptr chromosome) const
    {
        MultiValueChromosome<int, double>::ptr mvc = boost::dynamic_pointer_cast<MultiValueChromosome<int, double> >(
                chromosome);
        if (mvc)
        {
            //Let the fitness be the sum of all values
            return boost::shared_ptr<Fitness<double> >(new Fitness<double>(mvc->getSum()));
        }
        else
        {
            throw new std::runtime_error("Chromosome is not an Integer MultiValueChromosome with double fitness!");
        }

        boost::shared_ptr<Fitness<double> > ptr(new Fitness<double>(1));
        return ptr;
    }
};

int main(int argc, char **argv)
{

    std::cout << "Running GENEIAL demo1 - Version "
            << GENEIAL_VERSION_MAJOR
            << "." << GENEIAL_VERSION_MINOR
            << " ("
                << GENEIAL_BUILD_TYPE << ")"
            << std::endl;

    DemoChromosomeEvaluator::ptr evaluator(new DemoChromosomeEvaluator());

    ContinousMultiValueBuilderSettings<int, double> builderSettings(evaluator, 10, 130, 0, true, 20, 5);

    ContinousMultiValueChromosomeFactory<int,double> chromosomeFactory(builderSettings);

    MutationSettings mutationSettings(0.1, 0.1, 5);

    ChooseRandom<int, double> mutationChoosingOperation(mutationSettings);

    NonUniformMutationOperation<int, double> mutationOperation(1000, 0.2,
            mutationSettings, mutationChoosingOperation, builderSettings, chromosomeFactory);

    SelectionSettings selectionSettings(10);

    RouletteWheelSelection<double> selectionOperation(selectionSettings);

    CouplingSettings couplingSettings(20);

    RandomCouplingOperation<double> couplingOperation(couplingSettings);

    MultiValueChromosomeNPointCrossoverSettings crossoverSettings (1, MultiValueChromosomeNPointCrossoverSettings::RANDOM_WIDTH, 1);

    MultiValueChromosomeNPointCrossover<int, double> crossoverOperation(crossoverSettings, builderSettings, chromosomeFactory);

    BaseReplacementSettings replacementSettings(BaseReplacementSettings::REPLACE_ALL_OFFSPRING, 5, 2);

    ReplaceWorstOperation<double> replacementOperation(replacementSettings);

    MaxGenerationCriterion<double> stoppingCriterion(100000);

    SteadyStateAlgorithm<double> algorithm(
            stoppingCriterion, selectionOperation, couplingOperation, crossoverOperation, replacementOperation,
            mutationOperation, chromosomeFactory);

    algorithm.solve();

    std::cout << *algorithm.getHighestFitnessChromosome() << std::endl;

    std::cout << "end." << std::endl;

}
