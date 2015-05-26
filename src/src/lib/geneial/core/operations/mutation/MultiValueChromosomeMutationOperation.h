#pragma once

#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(mutation)
{
using ::geneial::population::chromosome::MultiValueChromosome;
using ::geneial::population::chromosome::MultiValueChromosomeFactory;

geneial_export_namespace
{
//The purpose of this class is to provide heirs the MVCFactory and to encapsulate high level mvc specific operations
template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeMutationOperation : public BaseMutationOperation<FITNESS_TYPE>
{

protected:
    std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> _builderFactory;

public:
    MultiValueChromosomeMutationOperation(
            const std::shared_ptr<const MutationSettings> &settings,
            const std::shared_ptr<const BaseChoosingOperation<FITNESS_TYPE>> &choosingOperation,
            const std::shared_ptr<MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>> &builderFactory
            ) :
            BaseMutationOperation<FITNESS_TYPE>(settings, choosingOperation), _builderFactory(builderFactory)
    {
    }

    virtual ~MultiValueChromosomeMutationOperation()
    {
    }

    inline MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> & getBuilderFactory() const
    {
        return *_builderFactory;
    }

};

} /* geneial_export_namespace */
} /* private namespace mutation */
} /* private namespace operation */
} /* private namespace geneial */

