#pragma once

#include <geneial/namespaces.h>

#include <geneial/algorithm/BaseGeneticAlgorithm.h>
#include <geneial/utility/mixins/EnableMakeShared.h>

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
using ::geneial::utility::EnableMakeShared;

geneial_export_namespace
{
class Diagnostics : public virtual EnableMakeShared<Diagnostics>{
    const std::shared_ptr<BaseBookkeeper> _bookkeeper;

protected:
    Diagnostics(const std::shared_ptr<BaseBookkeeper> &bookkeeper);
public:

    void analyseAll();

    void analyseTimeSpent();
    void analyseEventsCoupling();
    void analyseEventsCrossover();
    void analyseEventsMutation();
    void analyseEventsReplacement();
    void anaylseEventsSelection();

    class Builder
    {
    };
};


}
}
}
}
