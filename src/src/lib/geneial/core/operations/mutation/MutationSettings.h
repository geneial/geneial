/*
 * MutationSettings.h
 *
 *  Created on: Dec 28, 2014
 *      Author: lukas
 */

#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_MUTATIONSETTINGS_H_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_MUTATIONSETTINGS_H_

#include "assert.h"
namespace GeneticLibrary {
namespace Operation {
namespace Mutation {

class MutationSettings{
private:
	double _propabilityOfMutation; //propability of the mutation for one Chromosome
	//double _levelOfMutation; //amount of change caused to one chromosome
	double _amountOfMutation; //points of change in one chromosome

public:
	virtual ~MutationSettings(){};

	MutationSettings(double propabilityOfMutation, double amountOfMutation){
		_propabilityOfMutation = propabilityOfMutation;
		//_levelOfMutation = levelOfMutation;
		_amountOfMutation = amountOfMutation;
	};

	double getPropabilityOfMutation() const {
		return _propabilityOfMutation;
	};
/*
	double getLevelOfMutation() const {
		return _levelOfMutation;
	};
*/
	double getAmountOfMutation() const {
		return _amountOfMutation;
	};

	void setPropabilityOfMutation(double propabilityOfMutation){
		_propabilityOfMutation = propabilityOfMutation;
	};
/*
	void setLevelOfMutation(double levelOfMutation){
		_levelOfMutation = levelOfMutation;
	};
*/

	void setAmountOfMutation(double amountOfMutation){
		_amountOfMutation = amountOfMutation;
	};
};//Class MutationSettings


}//namespace Mutation
}//namespace Operation
}//namespace Genetic Library



#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_MUTATIONSETTINGS_H_ */
