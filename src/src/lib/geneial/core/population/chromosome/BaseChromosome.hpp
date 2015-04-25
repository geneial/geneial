#ifndef __GENEIAL_BASE_CHROMOSOME_HPP_
#define __GENEIAL_BASE_CHROMOSOME_HPP_

#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/core/fitness/Fitness.h>

#include <cstring> //memcmp

namespace geneial
{
namespace population
{
namespace chromosome
{

template<typename FITNESS_TYPE>
const typename Fitness<FITNESS_TYPE>::ptr BaseChromosome<FITNESS_TYPE>::getFitness()
{

    if (!hasFitnessEvaluator())
    {
        //No fitness evaluator -> undefined, probably null but no guarantee.
        return _fitness;
    }

    if (hasFitnessEvaluator() && !hasFitness())
    {
        //if we have a fitness evaluator assigned to the chromosome, but no fitness was calculated yet, do so.
        //_fitness = boost::shared_ptr<Fitness <FITNESS_TYPE> >(new Fitness<FITNESS_TYPE>(1));
        _fitness = _fitnessEvaluator->evaluate(this->getPtr());
    }

    return _fitness;

}

template<typename FITNESS_TYPE>
bool BaseChromosome<FITNESS_TYPE>::hashEquals(typename BaseChromosome<FITNESS_TYPE>::const_ptr chromosome) const
{
    const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashA = chromosome->getHash();
    const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashB = this->getHash();

    if (memcmp((void*) &hashA, (void*) &hashB, sizeof(hashA)) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template<typename FITNESS_TYPE>
void BaseChromosome<FITNESS_TYPE>::printHash(std::ostream& os) const
{
    os << this->getHash();
}

template<typename FITNESS_TYPE>
const typename Fitness<FITNESS_TYPE>::ptr BaseChromosome<FITNESS_TYPE>::getFitness() const
{
    return _fitness;
}

template<typename FITNESS_TYPE>
void BaseChromosome<FITNESS_TYPE>::setFitness(const typename Fitness<FITNESS_TYPE>::ptr& fit)
{
    _fitness = fit;
}

template<typename FITNESS_TYPE>
const typename FitnessEvaluator<FITNESS_TYPE>::ptr BaseChromosome<FITNESS_TYPE>::getFitnessEvaluator() const
{
    return _fitnessEvaluator;
}

template<typename FITNESS_TYPE>
void BaseChromosome<FITNESS_TYPE>::invalidateFitness()
{
    _fitness.reset(); //sets the ptr to null
}

template<typename FITNESS_TYPE>
void BaseChromosome<FITNESS_TYPE>::setFitnessEvaluator(
        const typename FitnessEvaluator<FITNESS_TYPE>::ptr& fitnessEvaluator)
{
    _fitnessEvaluator = fitnessEvaluator;
}

template<typename FITNESS_TYPE>
unsigned int BaseChromosome<FITNESS_TYPE>::doAge()
{
    return ++_age;
}

template<typename FITNESS_TYPE>
void BaseChromosome<FITNESS_TYPE>::setAge(unsigned int age)
{
    _age = age;
}

template<typename FITNESS_TYPE>
unsigned int BaseChromosome<FITNESS_TYPE>::getAge() const
{
    return _age;
}

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

#endif /* __GENEIAL_BASE_CHROMOSOME_HPP_ */
