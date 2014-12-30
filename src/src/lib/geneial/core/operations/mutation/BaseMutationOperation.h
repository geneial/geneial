/*
 * BaseMutationOperation.h
 *
 *  Created on: Dec 19, 2014
 *      Author: lukas
 */

#include <geneial/core/operations/mutation/MutationSettings.h>
#include <geneial/core/population/management/BaseManager.h>

using namespace GeneticLibrary::Population::Manager;

namespace GeneticLibrary {
namespace Operation {
namespace Mutation {

template <typename FITNESS_TYPE>
class BaseMutationOperation {
private:
	MutationSettings* _settings;
public:
	typedef typename Population::Population<FITNESS_TYPE>::chromosome_container mutation_result_set;
	BaseMutationOperation(MutationSettings *settings): _settings(settings) {}; //Constructor
	virtual ~BaseMutationOperation() {}; //Destructor

	virtual mutation_result_set doMutate
				(
						typename BaseMutationOperation<FITNESS_TYPE>::mutation_result_set,
						BaseMutationOperation<FITNESS_TYPE> *mutationOperation,
						BaseManager<FITNESS_TYPE> &manager
				) = 0;

	MutationSettings* const& getSettings() const {
		return _settings;
	};

	void setSettings(const MutationSettings*& settings) {
		_settings = settings;
	};
};

} //namespace Mutation
} //namespace Operation
} //namespace GeneticLibrary


