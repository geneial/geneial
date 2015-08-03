#include <geneial/core/population/management/Bookkeeper.h>

//TODO(bewo): Clean this up.

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(management)
{

geneial_export_namespace
{

    std::unique_ptr<ScopedTrace> StatisticBookkeeper::scopedEventTrace(const char* traceName)
    {
            return std::unique_ptr<ScopedTimeTrace>(new ScopedTimeTrace(this,traceName));
    }

}
}
}
}

