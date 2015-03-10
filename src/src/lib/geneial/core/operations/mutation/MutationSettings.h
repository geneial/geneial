#ifndef __GENEIAL_MUTATION_SETTINGS_H_
#define __GENEIAL_MUTATION_SETTINGS_H_

#include <cassert>

namespace geneial {
namespace operation {
namespace mutation {

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
		if (propability > 1)
		{
			propability = 1;
		}
		if (propability < 0)
		{
			propability = 0;
		}
		_probability = propability;
		_amountOfPointsOfMutation = amountOfPointsOfMutation;
		_amountOfMutation = amountOfMutation;
	}

	double getPropability() const
	{
		return _probability;
	}

	double getAmountOfMutation() const
	{
		return _amountOfMutation;
	}

	unsigned int getAmountOfPointsOfMutation()
	{
		return _amountOfPointsOfMutation;
	}

	void setAmountOfPointsOfMutation (unsigned int amount)
	{
		_amountOfPointsOfMutation = amount;
	}

	void setPropability(double propabilityOfMutation)
	{
		_probability = propabilityOfMutation;
	}
/*
	void setLevelOfMutation(double levelOfMutation){
		_levelOfMutation = levelOfMutation;
	};
*/

	void setAmountOfMutation(double amountOfMutation)
	{
		_amountOfMutation = amountOfMutation;
	}
};//Class MutationSettings


} /* namespace mutation */
} /* namespace operation */
} /* namespace geneial */



#endif /* __GENEIAL_MUTATION_SETTINGS_H_ */
