/*
 * MultiValueChromosomeFactory.h
 *
 *  Created on: Dec 9, 2014
 *      Author: bewo
 */

#ifndef MULTIVALUECHROMOSOMEFACTORY_H_
#define MULTIVALUECHROMOSOMEFACTORY_H_

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {
template <typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosomeFactory : public BaseChromosomeFactory<FITNESS_TYPE>{
protected:
	MultiValueBuilderSettings<VALUE_TYPE> *_settings;
public:
	MultiValueChromosomeFactory(MultiValueBuilderSettings<VALUE_TYPE> *settings):_settings(settings){};
	typename BaseChromosome<FITNESS_TYPE>::ptr createChromosome() = 0;

	const MultiValueBuilderSettings<VALUE_TYPE>*& getSettings() const {
		return _settings;
	}

	void setSettings(const MultiValueBuilderSettings<VALUE_TYPE>*& settings) {
		_settings = settings;
	}
};

} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */

#endif /* MULTIVALUECHROMOSOMEFACTORY_H_ */
