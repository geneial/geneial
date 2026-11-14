#pragma once

#include <geneial/core/operations/mutation/SmoothPeakMutationOperation.h>

#include <algorithm>
#include <iterator>
#include <numeric>
#include <unordered_set>
#include <utility>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(operation)
    {
        geneial_private_namespace(mutation)
        {
            using ::geneial::population::Population;
            using ::geneial::population::chromosome::BaseChromosomeFactory;
            using ::geneial::utility::Random;
            using ::geneial::utility::Smoothing;

            geneial_export_namespace
            {
                template <typename VALUE_TYPE, typename FITNESS_TYPE>
                typename Population<FITNESS_TYPE>::chromosome_container
                SmoothPeakMutationOperation<VALUE_TYPE, FITNESS_TYPE>::doMutate(
                    const typename Population<FITNESS_TYPE>::chromosome_container &chromosomeInputContainer,
                    BaseManager<FITNESS_TYPE> &manager) const
                {

                    typename Population<FITNESS_TYPE>::chromosome_container resultset;
                    typename Population<FITNESS_TYPE>::chromosome_container choosenChromosomeContainer;
                    typename Population<FITNESS_TYPE>::chromosome_container notChoosenChromosomeContainer;
                    const auto maxNumMvc = this->getBuilderFactory().getSettings().getNum();
                    if (this->getSettings().getMinimumPointsToMutate() >
                            this->getSettings().getMaximumPointsToMutate() ||
                        this->getSettings().getMaximumPointsToMutate() > maxNumMvc)
                        throw std::invalid_argument("Peak mutation count exceeds available positions");

                    const auto continousBuilderSettings =
                        (static_cast<const ContinousMultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE> &>(
                            this->getBuilderFactory().getSettings()));

                    choosenChromosomeContainer = this->getChoosingOperation().doChoose(chromosomeInputContainer);

                    for (auto it : chromosomeInputContainer)
                    {
                        auto result =
                            std::find(std::begin(choosenChromosomeContainer), std::end(choosenChromosomeContainer), it);
                        if (result == std::end(choosenChromosomeContainer))
                        {
                            notChoosenChromosomeContainer.emplace_back(it);
                        }
                    }

                    // only mutate choosen chromosomes
                    for (const auto &chosenChromosome : choosenChromosomeContainer)
                    {

                        const auto slotsToMutate =
                            Random::generate<unsigned int>(this->getSettings().getMinimumPointsToMutate(),
                                                           this->getSettings().getMaximumPointsToMutate());

                        // casting mutant to MVC
                        const auto mvcMutant =
                            std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>>(chosenChromosome);
                        if (!mvcMutant || mvcMutant->getSize() != maxNumMvc)
                            throw std::invalid_argument("Peak mutation requires matching chromosome dimensions");

                        // creating a new MVC (to keep things reversible)
                        auto mutatedChromosome =
                            std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>>(
                                this->getBuilderFactory().createChromosome(
                                    BaseChromosomeFactory<FITNESS_TYPE>::LET_UNPOPULATED));
                        assert(mutatedChromosome);

                        // getting values
                        const auto &mutantChromosomeContainer = std::as_const(*mvcMutant).getContainer();
                        auto &result_container = mutatedChromosome->getContainer();

                        std::copy(mutantChromosomeContainer.cbegin(), mutantChromosomeContainer.cend(),
                                  result_container.begin());

                        // Predetermine Positions for Mutation:
                        std::vector<unsigned int> positions(maxNumMvc);
                        std::iota(positions.begin(), positions.end(), 0u);
                        Random::shuffle(positions.begin(), positions.end());
                        positions.resize(slotsToMutate);

                        // We have an predetermined amount of points for introducing peaks...
                        for (const auto pos : positions)
                        {
                            const int sign = (Random::generateBit()) ? -1 : 1;
                            const auto peak = sign * Random::generate<VALUE_TYPE>(0, this->_maxElevation);
                            Smoothing::peakAt<VALUE_TYPE, FITNESS_TYPE>(
                                pos, Random::generate<unsigned int>(0, this->_maxLeftEps),
                                Random::generate<unsigned int>(0, this->_maxRightEps),
                                // TODO(bewo): Make minElevation another setting
                                peak, mutatedChromosome);
                        }

                        // Correct smoothness in mutated chromosome
                        Smoothing::restoreSmoothness<VALUE_TYPE, FITNESS_TYPE>(
                            mutatedChromosome, continousBuilderSettings.getEps(),
                            continousBuilderSettings.getRandomMin(), continousBuilderSettings.getRandomMax(),
                            continousBuilderSettings.hasStart(), continousBuilderSettings.getStartValue());

                        // Age reset
                        mutatedChromosome->setAge(0);
                        resultset.emplace_back(mutatedChromosome);
                    }

                    // add not mutated chromosomes
                    resultset.insert(resultset.end(), notChoosenChromosomeContainer.begin(),
                                     notChoosenChromosomeContainer.end());

                    return resultset;
                }

            } /* geneial_export_namespace */
        } /* private namespace mutation */
    } /* private namespace operation */
} /* private namespace geneial */
