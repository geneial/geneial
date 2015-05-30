#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/operations/coupling/BaseCouplingOperation.h>

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
using ::geneial::operation::coupling::BaseCouplingOperation;
using ::geneial::operation::selection::BaseSelectionOperation;
using ::geneial::population::management::BaseManager;

geneial_export_namespace
{

template<typename FITNESS_TYPE>
typename  BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set::size_type BaseCouplingOperation<FITNESS_TYPE>::copyUnlessMaximumReached(
        offspring_result_set &dest,
        const typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set &input,
        const typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set::size_type limit) const
{

    typedef typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set children_container;

    auto backInserter = std::back_inserter(dest);

    const auto toCopy = std::min<typename children_container::size_type>(input.size(),limit);

    std::move(input.begin(),input.begin()+toCopy,backInserter);

    return toCopy;
}

} /* geneial_export_namespace */
} /* private namespace coupling */
} /* private namespace operation */
} /* private namespace geneial */
