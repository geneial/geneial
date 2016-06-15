#include <geneial/algorithm/SteadyStateAlgorithm.h>

#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>

#include <geneial/core/population/builder/ContinousMultiValueBuilderSettings.h>
#include <geneial/core/population/builder/ContinousMultiValueChromosomeFactory.h>

#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>

#include <geneial/core/population/management/Bookkeeper.h>

#include <geneial/core/operations/choosing/ChooseRandom.h>
#include <geneial/core/operations/mutation/UniformMutationOperation.h>

#include <geneial/algorithm/criteria/CombinedCriterion.h>
#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>


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

using namespace geneial::operation::mutation;
using namespace geneial::operation::choosing;

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


int main2(int argc, char **argv)
{
    std::cout
            << "Running GENEIAL demo1 - Version "
            << GENEIAL_VERSION_STRING << " ("<<GENEIAL_BUILD_TYPE << ")"
            << std::endl;

    auto evaluator = std::make_shared<DemoChromosomeEvaluator>();


/*    ContinousMultiValueChromosomeFactory<int,double>::Builder factoryBuilder(evaluator);
    factoryBuilder.getSettings().setNum(20);
    factoryBuilder.getSettings().setRandomMin(20);
    factoryBuilder.getSettings().setRandomMax(130);
    factoryBuilder.getSettings().setHasStart(true);
    factoryBuilder.getSettings().setStartValue(70);
    factoryBuilder.getSettings().setEps(5);
*/

    //Factory:
    MultiValueChromosomeFactory<int, double>::Builder factoryBuilder(evaluator);
    factoryBuilder.getSettings().setNum(20);
    factoryBuilder.getSettings().setRandomMin(0);
    factoryBuilder.getSettings().setRandomMax(130);


    auto factory  = std::dynamic_pointer_cast<MultiValueChromosomeFactory<int, double>>(factoryBuilder.create());

    auto algorithmBuilder = SteadyStateAlgorithm<double>::Builder();
    algorithmBuilder.setChromosomeFactory(factory);

    UniformMutationOperation<int,double>::Builder mutationBuilder(factory);

    auto stoppingCriterion = std::make_shared<CombinedCriterion<double>>();
    stoppingCriterion->add(CombinedCriterion<double>::INIT,
	    std::make_shared<MaxGenerationCriterion<double>>(1000));
	algorithmBuilder.setStoppingCriterion(stoppingCriterion);



    auto choosing = ChooseRandom<double>::Builder().setProbability(0.5).create();
    mutationBuilder.setChoosingOperation(choosing);
    mutationBuilder.getSettings().setMinimumPointsToMutate(1);
    mutationBuilder.getSettings().setMaximumPointsToMutate(10);

    algorithmBuilder.setMutationOperation(mutationBuilder.create());

    auto algorithm = algorithmBuilder.create();

    algorithm->getPopulationSettings().setMaxChromosomes(100);

    //algorithm->setExecutionManager(std::move(std::unique_ptr<ThreadedExecutionManager>(new ThreadedExecutionManager(3))));
    //Diagnostics<double> diag(algorithm);

    algorithm->solve();

    //diag.analyseAll(std::cout);

    std::cout << *algorithm->getHighestFitnessChromosome() << std::endl;

    std::cout << "end." << std::endl;
    return 0;

}

int main(int argc, char **argv)
{
	for(int i=0;i<10;i++)
	{
		for(int i = 0; i < 1000; i++)
		{
		srand(1);
		}
		main2(argc,argv);
	}

	return 0;

}
