#pragma once

#include <geneial/core/operations/coupling/SimpleCouplingOperation.h>

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

geneial_export_namespace
{

template<typename FITNESS_TYPE>
typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set SimpleCouplingOperation<FITNESS_TYPE>::doCopulate(
        const typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &mating_pool,
        const BaseCrossoverOperation<FITNESS_TYPE> &crossoverOperation, BaseManager<FITNESS_TYPE> &manager)
{

    typedef typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set offspring_container;
    typedef typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set children_container;
    typedef typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set mating_container;
    typedef typename BaseChromosome<FITNESS_TYPE>::ptr chrom_ptr;

    unsigned int offspring_left = this->getSettings().getNumberOfOffspring();

    offspring_container offspring; //A small container for all the little children :-)
    offspring.reserve(offspring_left);


    assert(mating_pool.size() > 1);
    //iterate over mating candidates and create a pairwise mapping.
    /**
     * Example getNumberOfOffspring: 7, #mating_pool = 5
     *
     *  Parent1 \/ Child1
     *  Parent2 /\ Child2
     *
     *  Parent3 \/ Child3
     *  Parent4 /\ Child4
     *
     *  Parent5 \/ Child5
     *  Parent1 /\ Child6
     *
     *  Parent2 \/ Child7
     *  Parent3 /
     */
    for (typename mating_container::const_iterator it = mating_pool.begin(); offspring_left > 0;)
    {
        //TODO (bewo) REFACTOR: research cyclic iterators to avoid all those ugly case distinctions
        chrom_ptr parent1(*it);
        it++;
        //wrap around if necessary
        if (it == mating_pool.end())
        {
            it = mating_pool.begin();
        }

        chrom_ptr parent2(*it);
        it++;
        if (it == mating_pool.end())
        {
            it = mating_pool.begin();
        }

        const children_container children(crossoverOperation.doCrossover(parent1, parent2));
        offspring_left -= this->copyUnlessMaximumReached(offspring,children,offspring_left);

        if (offspring_left > 0 && !crossoverOperation.isSymmetric())
        {
            const children_container assymetricChildren(crossoverOperation.doCrossover(parent2, parent1));
            offspring_left -= this->copyUnlessMaximumReached(offspring,assymetricChildren,offspring_left);
        }
    }

    return offspring;
}

} /* geneial_export_namespace */
} /* private namespace coupling */
} /* private namespace operation */
} /* private namespace geneial */

