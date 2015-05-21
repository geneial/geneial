#pragma once

#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>


namespace geneial
{
namespace operation
{
namespace mutation
{

using namespace geneial::operation::choosing;
using namespace geneial::population::management;

//The purpose of this class is to provide heirs the MVCFactory and to encapsulate high level mvc specific operations
template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeMutationOperation : public BaseMutationOperation<FITNESS_TYPE>
{

protected:
    std::shared_ptr<const MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> _builderFactory;

public:
    MultiValueChromosomeMutationOperation(
            const std::shared_ptr<const MutationSettings> &settings,
            const std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation,
            const std::shared_ptr<const MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory
            ) :
            BaseMutationOperation<FITNESS_TYPE>(settings, choosingOperation), _builderFactory(builderFactory)
    {
    }

    virtual ~MultiValueChromosomeMutationOperation()
    {
    }

    const MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> & getBuilderFactory() const
    {
        return *_builderFactory;
    }

};

} /* namespace mutation */
} /* namespace operation */
} /* namespace geneial */

