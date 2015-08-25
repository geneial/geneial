#pragma once

#include <geneial/core/operations/crossover/MultiValueChromosomeAverageCrossover.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(crossover)
{
using ::geneial::population::Population;
using ::geneial::population::chromosome::MultiValueChromosome;
using ::geneial::operation::coupling::BaseCouplingOperation;

geneial_export_namespace
{

//TODO (bewo) allow random weighting factor param for one side
//value = (e.g. 2x ParentA + 1x ParentB) / 3 etc.
template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set MultiValueChromosomeAverageCrossover<VALUE_TYPE,
        FITNESS_TYPE>::doMultiValueCrossover(
        const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
        const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const
{

    typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set resultset;

    auto child_candidate = this->createChildCandidate();

    const auto &daddy_container = daddy->getContainer();
    const auto &mommy_container = mommy->getContainer();

    auto &child_container = child_candidate->getContainer();

    assert(daddy_container.size() == mommy_container.size());

    auto mommy_it(mommy_container.cbegin());
    auto daddy_it(daddy_container.cbegin());

    //TODO (bewo) is there a more elegant STL algorithm way to do this?!
    for (unsigned int i = 0; mommy_it != mommy_container.end(); ++i)
    {
        child_container[i] = ((*mommy_it + *daddy_it) / 2);
        //child_container.push_back(std::max(*mommy_it,*daddy_it));
        ++mommy_it;
        ++daddy_it;
    }

    resultset.emplace_back(std::move(child_candidate));
    return std::move(resultset);
}

} /* geneial_export_namespace */
} /* private namespace crossover */
} /* private namespace operation */
} /* private namespace geneial */

