#pragma once

#include <geneial/namespaces.h>

#include <geneial/algorithm/BaseGeneticAlgorithm.h>
#include <geneial/core/population/management/Bookkeeper.h>

#include <unordered_map>
#include <chrono>
#include <ctime>
#include <ratio>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{

geneial_export_namespace
{
using ::geneial::algorithm::BaseGeneticAlgorithm;

geneial_export_namespace
{

template<typename FITNESS_TYPE>
class Diagnostics
{
    const std::shared_ptr<BaseGeneticAlgorithm<FITNESS_TYPE>> _algorithm;

protected:
    void instrumentAlgorithm();

public:
    Diagnostics(const std::shared_ptr<BaseGeneticAlgorithm<FITNESS_TYPE>> &algorithm);

    void analyseAll(std::ostream& os);

    void analyseTimeSpent(std::ostream& os);
    void analyseEventsCoupling(std::ostream& os);
    void analyseEventsCrossover(std::ostream& os);
    void analyseEventsMutation(std::ostream& os);
    void analyseEventsReplacement(std::ostream& os);
    void anaylseEventsSelection(std::ostream& os);

};


}
}
}
}

#include <geneial/algorithm/diagnostics/Diagnostics.hpp>
