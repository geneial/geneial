/*
 * BaseMutationOperation.h
 *
 *  Created on: Dec 19, 2014
 *      Author: lukas
 */
#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_BASEMUTATIONOPERATION_H_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_BASEMUTATIONOPERATION_H_
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
   BaseMutationOperation(MutationSettings *settings): _settings(settings) {}; //Constructor
   virtual ~BaseMutationOperation() {}; //Destructor
   typedef typename Population::Population<FITNESS_TYPE>::chromosome_container mutation_result_set;
   virtual mutation_result_set doMutate
               (
            		   typename Mutation::BaseMutationOperation<FITNESS_TYPE>::mutation_result_set mutants
               ) =0;
   MutationSettings* const& getSettings() const {
       return _settings;
   };
   void setSettings(const MutationSettings*& settings) {
       _settings = settings;
   };
};//class
} //namespace Mutation
} //namespace Operation
} //namespace GeneticLibrary
#endif //SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_BASEMUTATIONOPERATION_H_
