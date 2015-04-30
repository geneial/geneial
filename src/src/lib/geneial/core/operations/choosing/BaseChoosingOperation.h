#pragma once

#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/population/Population.h>

namespace geneial
{
namespace operation
{
namespace choosing
{

using namespace geneial::population;

template<typename FITNESS_TYPE>
class BaseChoosingOperation
{
public:
    BaseChoosingOperation()
    {
    }
    virtual ~BaseChoosingOperation()
    {
    }

    virtual typename Population<FITNESS_TYPE>::chromosome_container doChoose(
            const typename Population<FITNESS_TYPE>::chromosome_container &chromosomeInputSet) const = 0;
};

} /* namespace choosing */
} /* namespace operation */
} /* namespace geneial */

