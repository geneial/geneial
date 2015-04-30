#pragma once

#include <geneial/core/operations/mutation/SmoothPeakMutationOperation.h>

namespace geneial
{
namespace operation
{
namespace mutation
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename Population<FITNESS_TYPE>::chromosome_container SmoothPeakMutationOperation<VALUE_TYPE, FITNESS_TYPE>::doMutate(
        const typename Population<FITNESS_TYPE>::chromosome_container &_chromosomeInputContainer,
        BaseManager<FITNESS_TYPE> &manager)
{

    typedef typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::value_container value_container;
    typedef typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr mvc_ptr;

    typename Population<FITNESS_TYPE>::chromosome_container resultset;
    typename Population<FITNESS_TYPE>::chromosome_container choosenChromosomeContainer;
    typename Population<FITNESS_TYPE>::chromosome_container notChoosenChromosomeContainer;

    choosenChromosomeContainer = this->getChoosingOperation()->doChoose(_chromosomeInputContainer);

    //calculates difference: _notChoosenChromosomeContainer = _choosenChromosomeContainer - _chromosomeInputContainer
    std::set_difference(_chromosomeInputContainer.begin(), _chromosomeInputContainer.end(),
            choosenChromosomeContainer.begin(), choosenChromosomeContainer.end(),
            std::inserter(notChoosenChromosomeContainer, notChoosenChromosomeContainer.begin()));

    typename Population<FITNESS_TYPE>::chromosome_container::iterator choosenChromosomeContainer_it;

    //only mutate choosen chromosomes
    for (choosenChromosomeContainer_it = choosenChromosomeContainer.begin();
            choosenChromosomeContainer_it != choosenChromosomeContainer.end(); ++choosenChromosomeContainer_it)
    {
        //casting mutant to MVC
        mvc_ptr mvcMutant = boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(
                *choosenChromosomeContainer_it);
        assert(mvcMutant);

        //creating a new MVC (to keep things reversible)
        mvc_ptr mutatedChromosome = boost::dynamic_pointer_cast<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> >(
                this->_builderFactory.createChromosome(BaseChromosomeFactory<FITNESS_TYPE>::LET_UNPOPULATED));
        assert(mutatedChromosome);

        //getting values
        value_container &mutantChromosomeContainer = mvcMutant->getContainer();
        value_container &result_container = mutatedChromosome->getContainer();

        //Copy over, result container is assumed to be empty at this point!
        std::copy(mutantChromosomeContainer.begin(), mutantChromosomeContainer.end(),
                std::back_inserter(result_container));

        //We have two choices here:
        // A) Mutate a fixed amount of given points (getAmountOfPointsOfMutation > 0)
        // B) Mutate every point based on a predetermined probability
        if (this->_settings.getAmountOfPointsOfMutation() > 0)
        {

            std::set<unsigned int> positionsToPeak;

            //We have an predetermined amount of points for introducing peaks...
            for (unsigned int i = this->_settings.getAmountOfPointsOfMutation(); i > 0; i--)
            {
                unsigned int pos;
                do
                {
                    pos = Random::generate<int>(0, this->_builderSettings.getNum() - 1);
                } while (positionsToPeak.find(pos) != positionsToPeak.end());

                const int sign = (Random::generateBit()) ? -1 : 1;
                Smoothing::peakAt<VALUE_TYPE, FITNESS_TYPE>(pos, Random::generate<int>(0, this->_maxLeftEps),
                        Random::generate<int>(0, this->_maxRightEps),
                        sign * Random::generate<int>(1, this->_maxElevation), mutatedChromosome);
            }
        }
        else
        {

            //Iterate the chromosome's value
            for (typename value_container::iterator it = mutatedChromosome->getContainer().begin();
                    it != mutatedChromosome->getContainer().end(); ++it)
            {
                const double value_choice = Random::generate<int>(0.0, 1.0);
                if (value_choice < this->_settings.getAmountOfMutation())
                {
                    const int sign = (Random::generateBit()) ? -1 : 1;
                    Smoothing::peakAt<VALUE_TYPE, FITNESS_TYPE>(
                            std::distance(mutatedChromosome->getContainer().begin(), it),
                            Random::generate<int>(0, this->_maxLeftEps),
                            Random::generate<int>(0, this->_maxRightEps),
                            sign * Random::generate<int>(1, this->_maxElevation), mutatedChromosome);
                }
            }

        }

        //Correct smoothness in mutated chromosome
        Smoothing::restoreSmoothness<VALUE_TYPE, FITNESS_TYPE>(mutatedChromosome, this->_builderSettings.getEps(),
                this->_builderSettings.getRandomMin(), this->_builderSettings.getRandomMax());

        //Age reset
        mutatedChromosome->setAge(0);
        resultset.push_back(mutatedChromosome);
    }

    //add not mutated chromosomes
    resultset.insert(resultset.end(), notChoosenChromosomeContainer.begin(), notChoosenChromosomeContainer.end());

    return resultset;

}

} /* namespace mutation */
} /* namespace operation */
} /* namespace geneial */

