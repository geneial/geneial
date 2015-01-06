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

	/**
	 * propability that one Chromosome is choosen for mutation
	 */

	double _propabilityOfMutation;

	/**
	 * _amountOfMutation defines the propability to choose a value in a chromosome.
	 */
	double _amountOfMutation;

	/**
	 * _amountOfPointsOfMutation defines at how many points, Mutation will change a chromosome.
	 * Does ignore _amountOfMutation.
	 * 0 Will turn of this feature, and switch to _amountOfMutation
	 */
	unsigned int _amountOfPointsOfMutation;

public:
	virtual ~MutationSettings(){};

	MutationSettings(double propabilityOfMutation, double amountOfMutation, unsigned int amountOfPointsOfMutation){
		_propabilityOfMutation = propabilityOfMutation;
		_amountOfPointsOfMutation = amountOfPointsOfMutation;
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

	unsigned int getAmountOfPointsOfMutation(){
		return _amountOfPointsOfMutation;
	};

	void setAmountOfPointsOfMutation (unsigned int amount){
		_amountOfPointsOfMutation = amount;
	}

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
