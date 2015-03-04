#ifndef __GENEIAL_POPULATION_SETTINGS_H_
#define __GENEIAL_POPULATION_SETTINGS_H_


namespace geneial {
namespace population {

class PopulationSettings {
private:
	unsigned int _max_chromosomes;

public:
	PopulationSettings(int max_chromosomes) :_max_chromosomes(max_chromosomes) {}
	virtual ~PopulationSettings() {}

	unsigned int getMaxChromosomes() const
	{
		return _max_chromosomes;
	}

	void setMaxChromosomes(int maxChromosomes)
	{
		_max_chromosomes = maxChromosomes;
	}
};

} /* namespace population */
} /* namespace geneial */


#endif /* __GENEIAL_POPULATION_SETTINGS_H_ */
