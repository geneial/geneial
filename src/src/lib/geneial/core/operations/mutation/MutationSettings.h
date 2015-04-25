#ifndef __GENEIAL_MUTATION_SETTINGS_H_
#define __GENEIAL_MUTATION_SETTINGS_H_

#include <cassert>

namespace geneial
{
namespace operation
{
namespace mutation
{

class MutationSettings
{
private:

    /**
     * probability that one chromosome is choosen for mutation
     */

    double _probability;

    /**
     * _amountOfMutation defines the probability to choose a value in a chromosome.
     */
    double _amountOfMutation;

    /**
     * _amountOfPointsOfMutation defines at how many points, mutation will change a chromosome.
     * Does ignore _amountOfMutation.
     * 0 Will turn of this feature, and switch to _amountOfMutation
     */
    unsigned int _amountOfPointsOfMutation;

public:
    virtual ~MutationSettings()
    {
    }

    MutationSettings(double probability, double amountOfMutation, unsigned int amountOfPointsOfMutation)
    {
        assert((probability <= 1) && (probability >= 0));
        _probability = probability;
        _amountOfPointsOfMutation = amountOfPointsOfMutation;
        _amountOfMutation = amountOfMutation;
    }

    double getPropability() const
    {
        return _probability;
    }

    double getAmountOfMutation() const
    {
        return _amountOfMutation;
    }

    unsigned int getAmountOfPointsOfMutation()
    {
        return _amountOfPointsOfMutation;
    }

    void setAmountOfPointsOfMutation(unsigned int amount)
    {
        _amountOfPointsOfMutation = amount;
    }

    void setPropability(double propabilityOfMutation)
    {
        _probability = propabilityOfMutation;
    }
    /*
     void setLevelOfMutation(double levelOfMutation){
     _levelOfMutation = levelOfMutation;
     };
     */

    void setAmountOfMutation(double amountOfMutation)
    {
        _amountOfMutation = amountOfMutation;
    }
};
//Class MutationSettings

} /* namespace mutation */
} /* namespace operation */
} /* namespace geneial */

#endif /* __GENEIAL_MUTATION_SETTINGS_H_ */
