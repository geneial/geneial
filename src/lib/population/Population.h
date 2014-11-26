/*
 * Population.h
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include "utility/printable.h"

using namespace GeneticLibrary::Utility;

namespace GeneticLibrary {
namespace Population {


class Population : public printable {
public:
	Population();
	virtual ~Population();
	virtual void print(std::ostream& os) const;
};

} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* POPULATION_H_ */
