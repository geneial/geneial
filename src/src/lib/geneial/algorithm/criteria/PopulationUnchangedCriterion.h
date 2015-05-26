#pragma once

#include <geneial/algorithm/criteria/StatefulStoppingCriterion.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
geneial_private_namespace(stopping_criteria)
{
using ::geneial::utility::Printable;
using ::geneial::population::management::BaseManager;

geneial_export_namespace
{
using namespace geneial::algorithm::stopping_criteria;

//TODO (bewo) Think about this class semantics...
template<typename FITNESS_TYPE>
class PopulationUnchangedCriterion: StatefulStoppingCriterion<FITNESS_TYPE>
{
public:
    PopulationUnchangedCriterion() :
            _wasEverInvoked(false), _lastGenerationOldestAge(0), _lastGenerationYoungestAge(0)
    {
    }

    virtual ~PopulationUnchangedCriterion()
    {
    }

    virtual bool wasStatefullyReached(BaseManager<FITNESS_TYPE> &manager)
    {
        bool result = false;

        if (!_wasEverInvoked)
        {
            _wasEverInvoked = true;
        }
        else
        {
            //TODO(bewo) is this correct?
            //Check the oldest age of the youngest chromosome against the previous run and if it has not changed abort
            result = (_lastGenerationOldestAge == manager.getPopulation().getYoungestChromosome()->getAge() - 1);
        }

        _lastGenerationOldestAge = manager.getPopulation().getOldestChromosome()->getAge();
        _lastGenerationYoungestAge = manager.getPopulation().getYoungestChromosome()->getAge();

        return result;
    }

    virtual void print(std::ostream& os) const
    {
        os << "Population Unchanged ()";
    }

private:
    //What was the last generation that was checked?
    unsigned int _wasEverInvoked;

    //Last generation age?
    unsigned int _lastGenerationOldestAge;
    unsigned int _lastGenerationYoungestAge;
};

} /* geneial_export_namespace */
} /* private namespace stopping_criteria */
} /* private namespace algorithm */
} /* private namespace geneial */
