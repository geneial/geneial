#ifndef __GENEIAL_MULTI_INT_VALUE_CHROMOSOME_FACTORY_H_
#define __GENEIAL_MULTI_INT_VALUE_CHROMOSOME_FACTORY_H_

#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>

namespace geneial
{
namespace population
{
namespace chromosome
{

//TODO (bewo): Instead of this weird class name rather specialize MultiValueChromosomeFactory for Integers
template<typename FITNESS_TYPE>
class MultiIntValueChromosomeFactory: public MultiValueChromosomeFactory<int, FITNESS_TYPE>
{

public:
    MultiIntValueChromosomeFactory(MultiValueBuilderSettings<int, FITNESS_TYPE> *settings) :
            MultiValueChromosomeFactory<int, FITNESS_TYPE>(settings)
    {
    }

    typename BaseChromosome<FITNESS_TYPE>::ptr createChromosome(
            typename BaseChromosomeFactory<FITNESS_TYPE>::PopulateBehavior populateValues = BaseChromosomeFactory<
                    FITNESS_TYPE>::CREATE_VALUES);

};

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

#include <geneial/core/population/builder/MultiIntValueChromosomeFactory.hpp>

#endif /* __GENEIAL_MULTI_INT_VALUE_CHROMOSOME_FACTORY_H_ */
