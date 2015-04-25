#ifndef __GENEIAL_BASE_SELECTION_OPERATION_H_
#define __GENEIAL_BASE_SELECTION_OPERATION_H_

#include <geneial/core/operations/selection/SelectionSettings.h>

namespace geneial
{
namespace operation
{
namespace selection
{

using namespace geneial::population::management;

/**
 * Select a number of parents based on a certain criteria.
 */
template<typename FITNESS_TYPE>
class BaseSelectionOperation
{
private:
    SelectionSettings* _settings;

public:
    typedef typename Population<FITNESS_TYPE>::chromosome_container selection_result_set;

    BaseSelectionOperation(SelectionSettings* settings) :
            _settings(settings)
    {
    }

    virtual ~BaseSelectionOperation()
    {
    }

    virtual selection_result_set doSelect(const Population<FITNESS_TYPE> &population,
            BaseManager<FITNESS_TYPE> &manager) = 0;

    SelectionSettings* getSettings() const
    {
        return _settings;
    }

    void setSettings(SelectionSettings* settings)
    {
        _settings = settings;
    }

};

} /* namespace selection */
} /* namespace operation */
} /* namespace geneial */

#endif /* __GENEIAL_BASE_SELECTION_OPERATION_H_ */
