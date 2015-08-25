#pragma once

#include <geneial/core/operations/coupling/RandomCouplingOperation.h>
#include <geneial/utility/Random.h>

#include <algorithm>
#include <iterator>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(coupling)
{
using ::geneial::population::Population;
using ::geneial::operation::crossover::BaseCrossoverOperation;
using ::geneial::operation::selection::BaseSelectionOperation;
using ::geneial::population::management::BaseManager;
using ::geneial::utility::Random;

geneial_export_namespace
{

template<typename FITNESS_TYPE>
typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set RandomCouplingOperation<FITNESS_TYPE>::doCopulate(
        const typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &mating_pool,
        const BaseCrossoverOperation<FITNESS_TYPE> &crossoverOperation, BaseManager<FITNESS_TYPE> &manager)
{

    typedef typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set offspring_container;
    typedef typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set children_container;
    typedef typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set mating_container;

    unsigned int offspring_left = this->getSettings().getNumberOfOffspring();

    offspring_container offspring; //A small container for all that little children,we're about to create :-)
    offspring.reserve(offspring_left);

    assert(mating_pool.size() > 1);

    //mating pool size:
    const unsigned int mating_pool_size = std::distance(mating_pool.begin(), mating_pool.end()) - 1;

    while (offspring_left)
    {
        //pick a random mommy:
        const unsigned int rnd_mommy = Random::generate<unsigned int>(0U, mating_pool_size);
        typename mating_container::const_iterator it_mommy = mating_pool.begin();
        std::advance(it_mommy, rnd_mommy);

        //pick a random daddy:
        unsigned int rnd_daddy;

        do
        {
            rnd_daddy = Random::generate<unsigned int>(0U, mating_pool_size);
        } while (rnd_daddy == rnd_mommy);

        typename mating_container::const_iterator it_daddy = mating_pool.begin();
        std::advance(it_daddy, rnd_daddy);


        //compute crossover
        const children_container children(crossoverOperation.doCrossover(*it_mommy, *it_daddy));
        offspring_left -= this->copyUnlessMaximumReached(offspring,children,offspring_left);
    }

    assert(offspring_left == 0);


    return offspring;
}

} /* geneial_export_namespace */
} /* private namespace coupling */
} /* private namespace operation */
} /* private namespace geneial */


