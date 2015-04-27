#pragma once

#include <geneial/algorithm/observer/AlgorithmObserver.h>

#include <set>

namespace geneial
{
namespace algorithm
{

using namespace geneial::population::management;

template<typename FITNESS_TYPE>
class BestChromosomeObserver: public AlgorithmObserver<FITNESS_TYPE>
{
public:
    BestChromosomeObserver() :
            _foundBest(false)
    {
        this->addSubscribedEvent(AlgorithmObserver<FITNESS_TYPE>::GENERATION_DONE);
    }
    virtual ~BestChromosomeObserver()
    {
    }

    virtual void updateGeneration(BaseManager<FITNESS_TYPE> &manager)
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

} /* namespace algorithm */
} /* namespace geneial */

