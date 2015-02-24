/*
 * MutationSettings.h
 *
 *  Created on: Dec 28, 2014
 *      Author: lukas
 */

#ifndef SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_MUTATIONSETTINGS_H_
#define SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_MUTATIONSETTINGS_H_

#include <cassert>

namespace GeneticLibrary {
namespace Operation {
namespace Mutation {

class MutationSettings{
private:

	/**
	 * propability that one Chromosome is choosen for mutation
	 */

	double _probability;

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

	MutationSettings(double propability, double amountOfMutation, unsigned int amountOfPointsOfMutation){
		_probability = propability;
		_amountOfPointsOfMutation = amountOfPointsOfMutation;
		_amountOfMutation = amountOfMutation;
	};

	double getPropability() const
	{
		return _probability;
	};

	double getAmountOfMutation() const
	{
		return _amountOfMutation;
	};

	unsigned int getAmountOfPointsOfMutation()
	{
		return _amountOfPointsOfMutation;
	};

	void setAmountOfPointsOfMutation (unsigned int amount)
	{
		_amountOfPointsOfMutation = amount;
	}

	void setPropability(double propabilityOfMutation)
	{
		_probability = propabilityOfMutation;
	};
/*
	void setLevelOfMutation(double levelOfMutation){
		_levelOfMutation = levelOfMutation;
	};
*/

	void setAmountOfMutation(double amountOfMutation)
	{
		_amountOfMutation = amountOfMutation;
	};
};//Class MutationSettings


}//namespace Mutation
}//namespace Operation
}//namespace Genetic Library



#endif /* SRC_LIB_GENEIAL_CORE_OPERATIONS_MUTATION_MUTATIONSETTINGS_H_ */
