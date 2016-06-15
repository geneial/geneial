#include <geneial/algorithm/SteadyStateAlgorithm.h>

#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>

#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>

#include <geneial/core/population/management/Bookkeeper.h>

#include <geneial/algorithm/diagnostics/Diagnostics.h>

#include <stdexcept>
#include <cassert>
#include <memory>

using namespace geneial;

using namespace geneial::algorithm;
using namespace geneial::utility;

using namespace geneial::algorithm::stopping_criteria;

using namespace geneial::population;
using namespace geneial::population::chromosome;

using geneial::population::management::StatisticBookkeeper;
using geneial::algorithm::Diagnostics;

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
            volatile float x = 1.5f;
            volatile int i = 1; //Increase i to simulate cpu fitness load ...
            while (i--)
            {
                x *= sin(x) / atan(x) * tanh(x) * sqrt(x);
            }


            const auto& mvc = dynamic_cast<const MultiValueChromosome<int, double>&>(chromosome);
            return std::unique_ptr<Fitness<double>>(new Fitness<double>(mvc.getSum()));
        }
        catch(std::bad_cast&)
        {
            throw new std::runtime_error("Chromosome is not an Integer MultiValueChromosome with double fitness!");
        }
        std::unique_ptr<Fitness<double>> ptr(new Fitness<double>);
        return ptr;
    }
};


int main(int argc, char **argv)
{
    std::cout
            << "Running GENEIAL demo1 - Version "
            << GENEIAL_VERSION_STRING << " ("<<GENEIAL_BUILD_TYPE << ")"
            << std::endl;

    auto evaluator = std::make_shared<DemoChromosomeEvaluator>();


    ContinousMultiValueChromosomeFactory<int,double>::Builder factoryBuilder(evaluator);
    factoryBuilder.getSettings().setNum(20);
    factoryBuilder.getSettings().setRandomMin(20);
    factoryBuilder.getSettings().setRandomMax(130);
    factoryBuilder.getSettings().setHasStart(true);
    factoryBuilder.getSettings().setStartValue(70);
    factoryBuilder.getSettings().setEps(5);

    auto algorithm = SteadyStateAlgorithm<double>::Builder().setChromosomeFactory(factoryBuilder.create()).create();

    algorithm->getPopulationSettings().setMaxChromosomes(100);

    //algorithm->setExecutionManager(std::move(std::unique_ptr<ThreadedExecutionManager>(new ThreadedExecutionManager(3))));
    //Diagnostics<double> diag(algorithm);

    algorithm->solve();

    //diag.analyseAll(std::cout);

    std::cout << *algorithm->getHighestFitnessChromosome() << std::endl;

    std::cout << "end." << std::endl;

}
