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
    unsigned int _holdoff_size;

public:
    explicit PopulationSettings(const unsigned int max_chromosomes = 100, const unsigned int holdoff_size=100) :
            _max_chromosomes(max_chromosomes),
            _holdoff_size(holdoff_size)
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

    unsigned int getHoldoffSize() const
    {
        return _holdoff_size;
    }

    void setHoldoffSize(const unsigned int holdoffSize)
    {
        _holdoff_size = holdoffSize;
    }
};

} /* geneial_export_namespace */
} /* private namespace population */
} /* private namespace geneial */
