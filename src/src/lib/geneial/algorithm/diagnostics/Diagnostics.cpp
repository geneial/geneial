#include <geneial/algorithm/diagnostics/Diagnostics.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
using ::geneial::algorithm::BaseGeneticAlgorithm;

geneial_export_namespace
{

Diagnostics::Diagnostics(const std::shared_ptr<BaseBookkeeper> &bookkeeper):_bookkeeper(bookkeeper)
{
}

inline void Diagnostics::analyseAll()
{
}

inline void Diagnostics::analyseTimeSpent()
{
}

inline void Diagnostics::analyseEventsCoupling()
{
}

inline void Diagnostics::analyseEventsCrossover()
{
}

inline void Diagnostics::analyseEventsMutation()
{
}

inline void Diagnostics::analyseEventsReplacement()
{
}

inline void Diagnostics::anaylseEventsSelection()
{
}

}
}
}
