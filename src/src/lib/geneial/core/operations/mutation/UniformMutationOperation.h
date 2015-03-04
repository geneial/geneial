#ifndef __GENEIAL_UNIFORM_MUTATION_OPERATION_H_
#define __GENEIAL_UNIFORM_MUTATION_OPERATION_H_

#include <geneial/core/operations/mutation/BaseMutationOperation.h>

#include <cassert>

namespace geneial {
namespace operation {
namespace mutation {

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class UniformMutationOperation: public BaseMutationOperation<FITNESS_TYPE>
{

private:
	MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> *_builderSettings;
	MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> *_builderFactory;
public:
	/*
	 * UniformMutationOperation Mutates a chromosome, by replacing some of it's values randomly.
	 */
	UniformMutationOperation(MutationSettings *settings,
			BaseChoosingOperation<FITNESS_TYPE> *choosingOperation,
			MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> *builderSettings,
			MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE> *builderFactory) :
			BaseMutationOperation<FITNESS_TYPE>(settings,choosingOperation),
					_builderSettings(builderSettings),
					_builderFactory(builderFactory)
	{
		assert(_builderSettings != NULL);
		assert(_builderFactory != NULL);
	}

	virtual ~UniformMutationOperation() {}

	/*
	 *  Returns a new chromosome which is a partially mutated version of the old one.
	 *  */
	virtual typename Population<FITNESS_TYPE>::chromosome_container doMutate(
			typename Population<FITNESS_TYPE>::chromosome_container mutants,
			BaseManager<FITNESS_TYPE> &manager
			);

	//TODO (lukas): copy paste from MultiValueChromosomeAvarageCrossover.h ...
	MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>* const & getBuilderFactory() const
	{
		return _builderFactory;
	}

}; //class

} /* namespace mutation */
} /* namespace operation */
} /* namespace geneial */

#include <geneial/core/operations/mutation/UniformMutationOperation.hpp>

#endif /* __GENEIAL_UNIFORM_MUTATION_OPERATION_H_ */
