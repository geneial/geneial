#pragma once

#include <geneial/core/operations/mutation/SmoothPeakMutationOperation.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(mutation)
{
using ::geneial::utility::Smoothing;
using ::geneial::population::chromosome::BaseChromosomeFactory;
using ::geneial::utility::Random;
using ::geneial::population::Population;

geneial_export_namespace
{
template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename Population<FITNESS_TYPE>::chromosome_container SmoothPeakMutationOperation<VALUE_TYPE, FITNESS_TYPE>::doMutate(
        const typename Population<FITNESS_TYPE>::chromosome_container &chromosomeInputContainer,
        BaseManager<FITNESS_TYPE> &manager) const
{

    typename Population<FITNESS_TYPE>::chromosome_container resultset;
    typename Population<FITNESS_TYPE>::chromosome_container choosenChromosomeContainer;
    typename Population<FITNESS_TYPE>::chromosome_container notChoosenChromosomeContainer;

    const auto continousBuilderSettings =
            (
            static_cast<const ContinousMultiValueBuilderSettings<VALUE_TYPE, FITNESS_TYPE>&>(this->getBuilderFactory().getSettings())
            );


    choosenChromosomeContainer = this->getChoosingOperation().doChoose(chromosomeInputContainer);

    //calculates difference: _notChoosenChromosomeContainer = _choosenChromosomeContainer - _chromosomeInputContainer
    std::set_difference(chromosomeInputContainer.cbegin(), chromosomeInputContainer.cend(),
            choosenChromosomeContainer.cbegin(), choosenChromosomeContainer.cend(),
            std::back_inserter(notChoosenChromosomeContainer));

    const auto slotsToMutate = Random::generate<unsigned int>(this->getSettings().getMinimumPointsToMutate(),this->getSettings().getMaximumPointsToMutate());

    //only mutate choosen chromosomes
    for (const auto &chosenChromosome : choosenChromosomeContainer)
    {

        const auto slotsToMutate = Random::generate<unsigned int>(this->getSettings().getMinimumPointsToMutate(),this->getSettings().getMaximumPointsToMutate());

        //casting mutant to MVC
        const auto mvcMutant = std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(
                chosenChromosome);
        assert(mvcMutant);

        //creating a new MVC (to keep things reversible)
        auto mutatedChromosome = std::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(
                this->getBuilderFactory().createChromosome(BaseChromosomeFactory<FITNESS_TYPE>::LET_UNPOPULATED));
        assert(mutatedChromosome);

        //getting values
        const auto &mutantChromosomeContainer = mvcMutant->getContainer();
        auto &result_container = mutatedChromosome->getContainer();


        result_container.clear();
        std::copy(mutantChromosomeContainer.cbegin(), mutantChromosomeContainer.cend(),
                result_container.begin());


        //We have two choices here:
        // A) Mutate a fixed amount of given points (getAmountOfPointsOfMutation > 0)
        // B) Mutate every point based on a predetermined probability
        if (slotsToMutate > 0)
        {

            std::set<unsigned int> positionsToPeak;

            //We have an predetermined amount of points for introducing peaks...
            for (unsigned int i = slotsToMutate; i > 0; i--)
            {
                unsigned int pos;
                do
                {
                    pos = Random::generate<int>(0, continousBuilderSettings.getNum() - 1);
                } while (positionsToPeak.find(pos) != positionsToPeak.end());

                const int sign = (Random::generateBit()) ? -1 : 1;
                Smoothing::peakAt<VALUE_TYPE, FITNESS_TYPE>(pos, Random::generate<int>(0, this->_maxLeftEps),
                        Random::generate<int>(0, this->_maxRightEps),
                        sign * Random::generate<int>(1, this->_maxElevation), mutatedChromosome);
            }
        }

        //Correct smoothness in mutated chromosome
        Smoothing::restoreSmoothness<VALUE_TYPE, FITNESS_TYPE>(
                mutatedChromosome,
                continousBuilderSettings.getEps(),
                continousBuilderSettings.getRandomMin(),
                continousBuilderSettings.getRandomMax());

        //Age reset
        mutatedChromosome->setAge(0);
        resultset.emplace_back(mutatedChromosome);
    }

    //add not mutated chromosomes
    resultset.insert(resultset.end(), notChoosenChromosomeContainer.begin(), notChoosenChromosomeContainer.end());

    return resultset;

}

} /* geneial_export_namespace */
} /* private namespace mutation */
} /* private namespace operation */
} /* private namespace geneial */

