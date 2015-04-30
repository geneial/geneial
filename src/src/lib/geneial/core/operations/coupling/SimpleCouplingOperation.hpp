#pragma once

#include <geneial/core/operations/coupling/SimpleCouplingOperation.h>

namespace geneial
{
namespace operation
{
namespace coupling
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

    unsigned int offspring_left = this.getSettings().getNumberOfOffspring();

    offspring_container offspring; //A small container for all the little children :-)
    offspring.reserve(offspring_left);


    assert(mating_pool.size() > 1);
    //iterate over mating candidates and create a pairwise mapping.
    /**
     * Example getNumberOfOffspring: 8, #mating_pool = 5
     *
     *  Parent1 \/ Child1
     *  Parent2 /\ Child2
     *
     *  Parent3 \/ Child3
     *  Parent4 /\ Child4
     *
     *  Parent5 \/ Child5
     *  Parent1 /\ Child7
     *
     *  Parent2 \/ Child8
     *  Parent3 /
     */
    for (typename mating_container::const_iterator it = mating_pool.begin(); offspring_left > 0;)
    {
        //TODO (bewo) REFACTOR: research cyclic iterators to avoid all those ugly case distinctions
        chrom_ptr parent1 = *it;
        it++;
        //wrap around if necessary
        if (it == mating_pool.end())
        {
            it = mating_pool.begin();
        }

        chrom_ptr parent2 = *it;
        it++;
        if (it == mating_pool.end())
        {
            it = mating_pool.begin();
        }

        auto backInserter = std::back_inserter(offspring);
        const children_container children = crossoverOperation.doCrossover(parent1, parent2);

        const typename children_container::size_type toCopy = std::min<typename children_container::size_type>(children.size(),offspring_left);
        std::copy_n(children.begin(), toCopy, backInserter);

        offspring_left -= toCopy;

        if (offspring_left > 0 && !crossoverOperation.isSymmetric())
        {
            const children_container assymetricChildren = crossoverOperation.doCrossover(parent2, parent1);

            const typename children_container::size_type assymetricToCopy = std::min<typename children_container::size_type>(assymetricChildren.size(),offspring_left);
            std::copy_n(assymetricToCopy.begin(),toCopy, backInserter));
            offspring_left -= toCopy;

        }
    }

    return offspring;
}

} /* namespace coupling */
} /* namespace operation */
} /* namespace geneial */

