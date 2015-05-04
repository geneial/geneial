#pragma once

#include <geneial/core/operations/crossover/MultiValueChromosomeAverageCrossover.h>

namespace geneial
{
namespace operation
{
namespace crossover
{

using namespace geneial::operation::coupling;

//TODO (bewo) allow random weighting factor param for one side
//value = (e.g. 2x ParentA + 1x ParentB) / 3 etc.
template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set MultiValueChromosomeAverageCrossover<VALUE_TYPE,
        FITNESS_TYPE>::doCrossover(const typename BaseChromosome<FITNESS_TYPE>::const_ptr &mommy,
        const typename BaseChromosome<FITNESS_TYPE>::const_ptr &daddy) const
{

    typedef typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::value_container value_container;
    typedef typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr mvc_cptr;
    typedef typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr mvc_ptr;

    typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set resultset;

    const mvc_cptr mvc_mommy = std::dynamic_pointer_cast<const MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(mommy);
    assert(mvc_mommy);

    const mvc_cptr mvc_daddy = std::dynamic_pointer_cast<const MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(daddy);
    assert(mvc_daddy);

    mvc_ptr child_candidate = std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(
            this->getBuilderFactory().createChromosome(BaseChromosomeFactory<FITNESS_TYPE>::LET_UNPOPULATED));
    assert(child_candidate);

    const value_container &daddy_container = mvc_daddy->getContainer();
    const value_container &mommy_container = mvc_mommy->getContainer();
    value_container &child_container = child_candidate->getContainer();

    assert(daddy_container.size() == mommy_container.size());

    typename value_container::const_iterator mommy_it(mommy_container.cbegin());
    typename value_container::const_iterator daddy_it(daddy_container.cbegin());

    for (unsigned int i = 0; mommy_it != mommy_container.end(); ++i)
    {
        child_container.push_back((*mommy_it + *daddy_it) / 2);
        //child_container.push_back(std::max(*mommy_it,*daddy_it));
        ++mommy_it;
        ++daddy_it;
    }

    resultset.push_back(child_candidate);
    return resultset;
}

} /* namespace crossover */
} /* namespace pperation */
} /* namespace geneial */

