#ifndef __GENEIAL_BUILDER_SETTINGS_H_
#define __GENEIAL_BUILDER_SETTINGS_H_

#include <geneial/core/fitness/FitnessEvaluator.h>

namespace geneial
{
namespace population
{
namespace chromosome
{

template<typename FITNESS_TYPE>
class BuilderSettings
{
private:
    typename FitnessEvaluator<FITNESS_TYPE>::ptr _fitnessEvaluator;

public:
    BuilderSettings(const typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator) :
            _fitnessEvaluator(fitnessEvaluator)
    {
    }

    virtual ~BuilderSettings()
    {
    }

    const typename FitnessEvaluator<FITNESS_TYPE>::ptr& getFitnessEvaluator() const
    {
        return _fitnessEvaluator;
    }

    void setFitnessEvaluator(const typename FitnessEvaluator<FITNESS_TYPE>::ptr& fitnessEvaluator)
    {
        _fitnessEvaluator = fitnessEvaluator;
    }

};

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

#endif /* __GENEIAL_BUILDER_SETTINGS_H_ */
