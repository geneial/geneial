#pragma once

namespace geneial
{
namespace population
{
namespace chromosome
{

template<typename FITNESS_TYPE>
class BaseChromosomeFactory
{
private:
public:

    enum PopulateBehavior
    {
        CREATE_VALUES, LET_UNPOPULATED
    };

    BaseChromosomeFactory()
    {
    }

    virtual ~BaseChromosomeFactory()
    {
    }

    virtual typename BaseChromosome<FITNESS_TYPE>::ptr createChromosome(
            PopulateBehavior populateValues = CREATE_VALUES) const = 0;
};

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

