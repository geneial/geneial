/*
 * BaseReplacementSettings.h
 *
 *  Created on: Dec 21, 2014
 *      Author: bewo
 */

#ifndef BASEREPLACEMENTSETTINGS_H_
#define BASEREPLACEMENTSETTINGS_H_

namespace GeneticLibrary {
namespace Operation {
namespace Replacement {

class BaseReplacementSettings{

public:
	/**
	 * Every selection operation should allow the following modes:
	 * 	REPLACE_ALL_OFFSPRING -- Replace every offspring
	 * 	REPLACE_FIXED_NUMBER -- Replace a given number in the existing Population with offspring
	 */
	typedef enum { REPLACE_ALL_OFFSPRING, REPLACE_FIXED_NUMBER } replace_offspring_mode;

	BaseReplacementSettings(replace_offspring_mode mode, unsigned int amountToReplace = 0, unsigned int amountElitism = 0) :
			_mode(mode), _amountToReplace(amountToReplace), _amountElitism(amountElitism){}

	unsigned int getAmountToReplace() const
	{
		return _amountToReplace;
	}

	void setAmountToReplace(unsigned int amountToReplace)
	{
		_amountToReplace = amountToReplace;
	}

	replace_offspring_mode getMode() const
	{
		return _mode;
	}

	void setMode(replace_offspring_mode mode)
	{
		_mode = mode;
	}

	unsigned int getAmountElitism() const
	{
		return _amountElitism;
	}

	void setAmountElitism(unsigned int amountElitism)
	{
		_amountElitism = amountElitism;
	}
private:
	replace_offspring_mode _mode;
	unsigned int _amountToReplace;
	unsigned int _amountElitism;

};

} /* namespace Replacement */
} /* namespace Operation */
} /* namespace GeneticLibrary */

#endif /* BASEREPLACEMENTSETTINGS_H_ */
