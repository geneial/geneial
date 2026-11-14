#pragma once

#include <geneial/namespaces.h>
#include <stdexcept>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(population)
    {
        geneial_export_namespace
        {

            class PopulationSettings
            {
              private:
                unsigned int _max_chromosomes;
                unsigned int _holdoff_size;
                unsigned int _maxReplenishAttempts = 10000;

              public:
                explicit PopulationSettings(const unsigned int max_chromosomes = 100,
                                            const unsigned int holdoff_size = 100)
                    : _max_chromosomes(max_chromosomes), _holdoff_size(holdoff_size)
                {
                    if (max_chromosomes == 0)
                        throw std::invalid_argument("Population size must be positive");
                }

                virtual ~PopulationSettings() {}

                unsigned int getMaxChromosomes() const
                {
                    return _max_chromosomes;
                }

                void setMaxChromosomes(int maxChromosomes)
                {
                    if (maxChromosomes <= 0)
                    {
                        throw std::invalid_argument("Population size must be positive");
                    }
                    _max_chromosomes = maxChromosomes;
                }

                unsigned int getMaxReplenishAttempts() const
                {
                    return _maxReplenishAttempts;
                }

                void setMaxReplenishAttempts(unsigned int attempts)
                {
                    if (attempts == 0)
                    {
                        throw std::invalid_argument("Replenishment attempt limit must be positive");
                    }
                    _maxReplenishAttempts = attempts;
                }

                unsigned int getHoldoffSize() const
                {
                    return _holdoff_size;
                }

                void setHoldoffSize(const unsigned int holdoffSize)
                {
                    _holdoff_size = holdoffSize;
                }
            };

        } /* geneial_export_namespace */
    } /* private namespace population */
} /* private namespace geneial */
