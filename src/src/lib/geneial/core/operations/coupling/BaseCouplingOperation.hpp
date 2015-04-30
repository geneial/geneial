#pragma once

#include <geneial/core/operations/coupling/BaseCouplingOperation.h>

#include <algorithm>
#include <iterator>

namespace geneial
{
namespace operation
{
namespace coupling
{

using namespace geneial::operation::selection;
using namespace geneial::operation::crossover;
using namespace geneial::population::management;

template<typename FITNESS_TYPE>
typename  BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set::size_type BaseCouplingOperation<FITNESS_TYPE>::copyUnlessMaximumReached(
        offspring_result_set &dest,
        const typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set &input,
        const typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set::size_type limit) const
{

    typedef typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set children_container;

    auto backInserter = std::back_inserter(dest);

    const typename children_container::size_type toCopy
    = std::min<typename children_container::size_type>(input.size(),limit);

    std::copy_n(input.begin(), toCopy, backInserter);

    return toCopy;
}

}
/* namespace coupling */
} /* namespace operation */
} /* namespace geneial */

