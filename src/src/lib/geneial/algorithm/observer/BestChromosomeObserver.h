#pragma once

#include <geneial/algorithm/observer/AlgorithmObserver.h>

#include <set>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{
using ::geneial::population::management::BaseManager;

geneial_export_namespace
{


template<typename FITNESS_TYPE>
class BestChromosomeObserver: public AlgorithmObserver<FITNESS_TYPE>
{
public:
    BestChromosomeObserver() :
            _foundBest(false)
    {
        this->addSubscribedEvent(AlgorithmObserver<FITNESS_TYPE>::AFTER_GENERATION);
    }
    virtual ~BestChromosomeObserver()
    {
    }

    virtual void updateAfterGeneration(BaseManager<FITNESS_TYPE> &manager) override
    {
        const FITNESS_TYPE lastBest = manager.getHighestFitness();
        if (lastBest > _best || !_foundBest)
        {
            _best = lastBest;
            _foundBest = true;
            updateNewBestChromosome(manager);

        }
    }

    virtual void updateNewBestChromosome(BaseManager<FITNESS_TYPE> &manager)
    {
    }

private:
    FITNESS_TYPE _best;

    bool _foundBest;
};


} /* geneial_export_namespace */
} /* private namespace algorithm */
} /* private namespace geneial */

