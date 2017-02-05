#include <geneial/algorithm/SteadyStateAlgorithm.h>

#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>

#include <geneial/core/population/builder/PermutationChromosomeFactory.h>

#include <geneial/core/operations/crossover/MultiValuePermutationCrossover.h>
#include <geneial/core/operations/mutation/ValueSwapMutationOperation.h>

#include <geneial/core/population/management/Bookkeeper.h>

#include <geneial/algorithm/criteria/CombinedCriterion.h>
#include <geneial/algorithm/criteria/MaxGenerationCriterion.h>

#include <geneial/algorithm/diagnostics/Diagnostics.h>

#include <geneial/utility/mixins/Hasher.h>

#include <stdexcept>
#include <cassert>
#include <memory>
#include <unordered_map>

using namespace geneial;

using namespace geneial::algorithm;
using namespace geneial::utility;

using namespace geneial::algorithm::stopping_criteria;

using namespace geneial::population;
using namespace geneial::population::chromosome;

using namespace geneial::operation::crossover;
using namespace geneial::operation::mutation;

using geneial::population::management::StatisticBookkeeper;
using geneial::algorithm::Diagnostics;



class TSPNodeDistanceTable
{
private:
	std::unordered_map<std::pair<int,int>,double,geneial::utility::PairHasher> _distances;

public:
	void registerDistance(const int idxPointStart, const int idxPointEnd, double distance)
	{
		_distances.emplace(std::make_pair(idxPointStart,idxPointEnd),distance);
	}

	double getDistance(const int idxPointStart, const int idxPointEnd)
	{
		auto search = _distances.find(std::make_pair(idxPointStart,idxPointEnd));
	    if(search == _distances.end()) {
	    	throw std::invalid_argument("Unable to find Distance for given Pair!");
	    }
		return search->second;
	}

	double getTotalDistanceForPath(const std::vector<int>& indices)
	{
		double distanceTotal = 0.0;
		if(indices.size() > 1)
		{
			for(unsigned int i=0; i < indices.size() - 1; i++)
			{
				distanceTotal += getDistance(indices[i],indices[i+1]);
			}
		}
		return distanceTotal;
	}

};


class TSPSolutionEvaluator: public FitnessEvaluator<double>
{

private:
	std::shared_ptr<TSPNodeDistanceTable> _distanceTable;

public:
	TSPSolutionEvaluator(std::shared_ptr<TSPNodeDistanceTable> distances):_distanceTable(distances)
    {
    }

    std::unique_ptr<Fitness<double>> evaluate(const BaseChromosome<double>& chromosome) const
    {
        try
        {
            const auto& mvc = dynamic_cast<const MultiValueChromosome<int, double>&>(chromosome);
            const double distance = _distanceTable->getTotalDistanceForPath(mvc.getContainer());
            return std::unique_ptr<Fitness<double>>(new Fitness<double>(-distance));
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
            << "Running GENEIAL TSP Demo - Version "
            << GENEIAL_VERSION_STRING << " ("<<GENEIAL_BUILD_TYPE << ")"
            << std::endl;

    auto distanceTable = std::make_shared<TSPNodeDistanceTable>();
    std::vector<double> distances = {
									   0, 20, 42, 35, //1
									   20, 0, 30, 34, //2
									   42, 30, 0, 12, //3
									   35, 34, 12, 0  //4
									  };
    auto num_nodes = std::sqrt(distances.size());
    for(int i=0;i<num_nodes;i++)
    {
        for(int j=0;j<num_nodes;j++)
        {
        	distanceTable->registerDistance(i,j,distances[num_nodes*i+j]);
        }
    }



    auto evaluator = std::make_shared<TSPSolutionEvaluator>(distanceTable);
    PermutationChromosomeFactory<int,double>::Builder factoryBuilder(evaluator);
    //How many times should new chromosomes be shuffled around?
    factoryBuilder.getSettings().setPermutationRoundsMin(num_nodes);
    factoryBuilder.getSettings().setPermutationRoundsMax(2*num_nodes);

    //How many values to permutate?
    factoryBuilder.getSettings().setNum(num_nodes);

    auto factory = std::dynamic_pointer_cast<MultiValueChromosomeFactory<int, double>>(factoryBuilder.create());


    //Crossover:
	auto permutationCrossover = MultiValuePermutationCrossover<int,double>::Builder(factory).create();

	//Mutation:
	auto permutationMutationBuilder = ValueSwapMutationOperation<int,double>::Builder(factory);
	permutationMutationBuilder.getSettings().setMinimumPointsToMutate(1);
	permutationMutationBuilder.getSettings().setMaximumPointsToMutate(1);

    //Stopping Criteria
    auto stoppingCriterion = std::make_shared<CombinedCriterion<double>>();
    stoppingCriterion->add(CombinedCriterion<double>::INIT,
	    std::make_shared<MaxGenerationCriterion<double>>(num_nodes*100));


    auto algorithm = SteadyStateAlgorithm<double>::Builder()
    		.setChromosomeFactory(factory)
			.setCrossoverOperation(permutationCrossover)
			.setMutationOperation(permutationMutationBuilder.create())
			.setStoppingCriterion(stoppingCriterion)
		.create();

    algorithm->getPopulationSettings().setMaxChromosomes(num_nodes*2);


    Diagnostics<double> diag(algorithm);

    algorithm->solve();

    diag.analyseAll(std::cout);

    std::cout << *algorithm->getHighestFitnessChromosome() << std::endl;

    std::cout << "end." << std::endl;

}
