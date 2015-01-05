/*
 * UniformMutationOperation.h
 *
 *  Created on: Dec 28, 2014
 *      Author: lukas
 */
#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_UNIFORMMUTATIONOPERATION_H_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_UNIFORMMUTATIONOPERATION_H_
#include <geneial/core/operations/mutation/BaseMutationOperation.h>
#include "assert.h"
namespace GeneticLibrary {
namespace Operation {
namespace Mutation {
using namespace GeneticLibrary::Population::Manager;
template <typename VALUE_TYPE,typename FITNESS_TYPE>
class UniformMutationOperation : public BaseMutationOperation<FITNESS_TYPE> {
private:
   MultiValueBuilderSettings<VALUE_TYPE,FITNESS_TYPE> * _builderSettings;
   MultiValueChromosomeFactory<VALUE_TYPE,FITNESS_TYPE> *_builderFactory;
public:
   /*
    * UniformMutationOperation Mutates a chromosome, by replacing some of it's values randomly.
    */
   UniformMutationOperation
       (
           MutationSettings *settings,
           MultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> *builderSettings,
           MultiValueChromosomeFactory<VALUE_TYPE,FITNESS_TYPE> *builderFactory
       ):
           BaseMutationOperation<FITNESS_TYPE>(settings),
           _builderSettings(builderSettings),
           _builderFactory(builderFactory)
       {
       assert(_builderSettings != NULL);
       assert(_builderFactory != NULL);
   };
   virtual ~UniformMutationOperation() {};
   virtual typename BaseMutationOperation<FITNESS_TYPE>::mutation_result_set doMutate(double min, double max, typename Population::Chromosome::BaseChromosome<FITNESS_TYPE>::ptr mutant);
   //TODO lukas copy paste from MultiValueChromosomeAvarageCrossover.h ...
   MultiValueChromosomeFactory<VALUE_TYPE, FITNESS_TYPE>* const & getBuilderFactory() const {
       return _builderFactory;
   }
};//class
} // namespace Mutation
} // namespace Operation
} // namespace GeneticLibrary
#include <geneial/core/operations/mutation/UniformMutationOperation.hpp>
#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_UNIFORMMUTATIONOPERATION_H_ */
