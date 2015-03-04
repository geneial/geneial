#ifndef __GENEIAL_BASE_COUPLING_OPERATION_H_
#define __GENEIAL_BASE_COUPLING_OPERATION_H_

#include <geneial/core/operations/coupling/CouplingSettings.h>
#include <geneial/core/operations/selection/BaseSelectionOperation.h>
#include <geneial/core/operations/crossover/BaseCrossoverOperation.h>
#include <geneial/core/population/management/BaseManager.h>


namespace geneial {
namespace operation {
namespace coupling {

using namespace geneial::operation::selection;
using namespace geneial::operation::crossover;
using namespace geneial::population::management;


template <typename FITNESS_TYPE>
class BaseCouplingOperation {

private:
	CouplingSettings* _settings;

public:
	typedef typename Population<FITNESS_TYPE>::chromosome_container offspring_result_set;
	BaseCouplingOperation(CouplingSettings *settings): _settings(settings)  {};
	virtual ~BaseCouplingOperation() {};

	virtual offspring_result_set doCopulate
				(
						typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &mating_pool,
						BaseCrossoverOperation<FITNESS_TYPE> *crossoverOperation,
						BaseManager<FITNESS_TYPE> &manager
				) = 0;

	CouplingSettings* const& getSettings() const
	{
		return _settings;
	};

	void setSettings(const CouplingSettings*& settings)
	{
		_settings = settings;
	};

};

} /* namespace coupling */
} /* namespace operation */
} /* namespace geneial */

#endif /* __GENEIAL_BASE_COUPLING_OPERATION_H_ */
