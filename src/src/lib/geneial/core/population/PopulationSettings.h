#pragma once

#include <geneial/namespaces.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_export_namespace
{

class PopulationSettings
{
private:
    unsigned int _max_chromosomes;

public:
    explicit PopulationSettings(int max_chromosomes = 100) :
            _max_chromosomes(max_chromosomes)
    {
    }

    virtual ~PopulationSettings()
    {
    }

    unsigned int getMaxChromosomes() const
    {
        return _max_chromosomes;
    }

    void setMaxChromosomes(int maxChromosomes)
    {
        _max_chromosomes = maxChromosomes;
    }
};

} /* geneial_export_namespace */
} /* private namespace population */
} /* private namespace geneial */
