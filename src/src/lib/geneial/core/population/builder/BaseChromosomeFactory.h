#ifndef __GENEIAL_BASE_CHROMOSOME_FACTORY_H_
#define __GENEIAL_BASE_CHROMOSOME_FACTORY_H_

namespace geneial {
namespace population {
namespace chromosome {

template <typename FITNESS_TYPE>
class BaseChromosomeFactory
{
private:
public:
	BaseChromosomeFactory() {};
	virtual ~BaseChromosomeFactory() {}
	virtual typename BaseChromosome<FITNESS_TYPE>::ptr createChromosome(bool populateValues = true) = 0;
};

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

#endif /* __GENEIAL_BASE_CHROMOSOME_FACTORY_H_ */
