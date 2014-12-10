/*
 * Fitness.h
 *
 *  Created on: Dec 8, 2014
 *      Author: bewo
 */

#ifndef FITNESS_H_
#define FITNESS_H_

#include <utility/printable.h>
namespace GeneticLibrary{

template <typename FITNESS_TYPE>
class Fitness : public GeneticLibrary::Utility::printable
{
private:
	FITNESS_TYPE _value;
  public:
	Fitness() {};

	virtual ~Fitness() {};

	void set(FITNESS_TYPE value);

	FITNESS_TYPE get() const;

    operator FITNESS_TYPE() const { return get();};

    void print(std::ostream&) const;
};

} /* namespace GeneticLibrary */
#include <core/fitness/Fitness.hpp>
#endif /* FITNESS_H_ */
