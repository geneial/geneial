#include <geneial/algorithm/SteadyStateAlgorithm.h>

#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>

#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>

#include <geneial/core/population/management/Bookkeeper.h>

#include <geneial/config.h>

#include <stdexcept>
#include <cassert>
#include <memory>
#include <chrono>
#include <thread>

#include <unistd.h>

using namespace geneial;

using namespace geneial::algorithm;
using namespace geneial::utility;

using namespace geneial::algorithm::stopping_criteria;

using namespace geneial::population;
using namespace geneial::population::chromosome;

using geneial::population::management::StatisticBookkeeper;

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
            float x = 1.5f;
            int i = 1;
            while (i--)
            {
                x *= sin(x) / atan(x) * tanh(x) * sqrt(x);
            }

            const MultiValueChromosome<int, double>& mvc = dynamic_cast<const MultiValueChromosome<int, double>&>(chromosome);
            //usleep(1);
            return std::move(std::unique_ptr<Fitness<double>>(new Fitness<double>(mvc.getSum())));
        }
        catch(std::bad_cast&)
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

    auto algorithm = SteadyStateAlgorithm<double>::Builder().
            setChromosomeFactory(std::make_shared<ContinousMultiValueChromosomeFactory<int,double>>(builderSettings)).
            build();

    algorithm->getPopulationSettings().setMaxChromosomes(100);

    //algorithm->setExecutionManager(std::move(std::unique_ptr<ThreadedExecutionManager>(new ThreadedExecutionManager(3))));

    algorithm->setBookkeeper(std::move(std::unique_ptr<StatisticBookkeeper>(new StatisticBookkeeper)));

    algorithm->solve();

    std::cout << *algorithm->getHighestFitnessChromosome() << std::endl;

    std::cout << "end." << std::endl;

}
