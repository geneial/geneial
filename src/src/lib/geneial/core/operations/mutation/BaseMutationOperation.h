#ifndef __GENEIAL_BASE_MUTATION_OPERATION_H_
#define __GENEIAL_BASE_MUTATION_OPERATION_H_

#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/core/population/Population.h>
#include <geneial/core/operations/choosing/BaseChoosingOperation.h>

namespace geneial
{
namespace operation
{
namespace mutation
{

using namespace geneial::operation::choosing;
using namespace geneial::population::management;

template<typename FITNESS_TYPE>
class BaseMutationOperation
{
private:
    MutationSettings* _settings;
    BaseChoosingOperation<FITNESS_TYPE>* _choosingOperation;
public:
    //Constructor
    BaseMutationOperation(MutationSettings *settings, BaseChoosingOperation<FITNESS_TYPE> *choosingOperation) :
            _settings(settings), _choosingOperation(choosingOperation)
    {
    }

    //Destructor
    virtual ~BaseMutationOperation()
    {
    }

    typedef typename Population<FITNESS_TYPE>::chromosome_container mutation_result_set;
    virtual typename Population<FITNESS_TYPE>::chromosome_container doMutate(
            typename Population<FITNESS_TYPE>::chromosome_container mutants, BaseManager<FITNESS_TYPE> &manager) = 0;

    MutationSettings* const & getSettings() const
    {
        return _settings;
    }

    void setSettings(const MutationSettings*& settings)
    {
        _settings = settings;
    }

    BaseChoosingOperation<FITNESS_TYPE>* getChoosingOperation() const
    {
        return _choosingOperation;
    }
};

} /* namespace mutation */
} /* namespace operation */
} /* namespace geneial */

#endif /* __GENEIAL_BASE_MUTATION_OPERATION_H_ */
