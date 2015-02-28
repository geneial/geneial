#ifndef CONTAINER_TYPES_H_
#define CONTAINER_TYPES_H_
#include <vector>

namespace GeneticLibrary {
namespace Population {

template <typename FITNESS_TYPE>
class ContainerTypes
{
private:
	ContainerTypes(){};
public:
	//A generic container to pass chromomsomes between operations.
	typedef typename std::vector<typename Chromosome::BaseChromosome<FITNESS_TYPE>::ptr> chromosome_container;
};

} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* CONTAINER_TYPES_H_ */
