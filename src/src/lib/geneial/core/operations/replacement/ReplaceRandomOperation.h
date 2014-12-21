/*
 * ReplaceRandomOperation.h
 *
 *  Created on: Dec 21, 2014
 *      Author: bewo
 */

#ifndef REPLACERANDOMOPERATION_H_
#define REPLACERANDOMOPERATION_H_

#include <geneial/core/operations/replacement/BaseReplacementOperation.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Operation {
namespace Replacement {
/**
 * Select a number of parents based on a certain criteria.
 */
template <typename FITNESS_TYPE>
class ReplaceRandomOperation : public BaseReplacementOperation<FITNESS_TYPE>{
private:
	unsigned int getAmountToReplace(
			const typename Population::Population<FITNESS_TYPE> &population,
			const typename Coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring) const;
public:
	ReplaceRandomOperation(BaseReplacementSettings* settings): BaseReplacementOperation<FITNESS_TYPE>(settings){};
	virtual ~ReplaceRandomOperation() {};

	void doReplace(
			typename Population::Population<FITNESS_TYPE> &population,
			typename Selection::BaseSelectionOperation<FITNESS_TYPE>::selection_result_set &parents,
			typename Coupling::BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set &offspring,
			BaseManager<FITNESS_TYPE> &manager);

};

} /* namespace Replacement */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#include <geneial/core/operations/replacement/ReplaceRandomOperation.hpp>

#endif /* REPLACERANDOMOPERATION_H_ */
