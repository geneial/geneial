#pragma once

namespace geneial
{
namespace population
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

} /* namespace population */
} /* namespace geneial */
