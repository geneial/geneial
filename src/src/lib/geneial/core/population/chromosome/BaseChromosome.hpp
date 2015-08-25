#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/core/fitness/Fitness.h>

#include <boost/version.hpp>

#include <cstring> //memcmp

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{

geneial_export_namespace
{
template<typename FITNESS_TYPE>
const Fitness<FITNESS_TYPE>& BaseChromosome<FITNESS_TYPE>::getFitness() const
{

    //Note that _fitness is mutable and should be only computed if needed.

    //Note that _fitness is const wrt. the chromosome contents, only the actual computation is held off until firstly needed.

    if (_fitnessEvaluator && !hasFitness())
    {
        //if we have a fitness evaluator assigned to the chromosome, but no fitness was calculated yet, do so.
        assert(_fitnessEvaluator);
        assert(this);
        _fitness = (std::move(_fitnessEvaluator->evaluate(*this)));
    }

    //No fitness evaluator -> undefined, probably null but no guarantee.
    return *_fitness;

}

template<typename FITNESS_TYPE>
bool BaseChromosome<FITNESS_TYPE>::hashEquals(const BaseChromosome<FITNESS_TYPE> &chromosome) const
{
    const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashA = chromosome.getHash();
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
void BaseChromosome<FITNESS_TYPE>::setFitness(typename std::unique_ptr<Fitness<FITNESS_TYPE>> fit)
{
    _fitness = std::move(fit);
}

template<typename FITNESS_TYPE>
const typename FitnessEvaluator<FITNESS_TYPE>::ptr BaseChromosome<FITNESS_TYPE>::getFitnessEvaluator() const
{
    return _fitnessEvaluator;
}

template<typename FITNESS_TYPE>
void BaseChromosome<FITNESS_TYPE>::invalidateFitness()
{
    _fitness = nullptr;
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

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */

