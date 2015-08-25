#pragma once

#include <geneial/core/fitness/Fitness.h>
#include <geneial/core/fitness/FitnessEvaluator.h>
#include <geneial/core/population/chromosome/MultiValueChromosome.h>

#include "MockFitnessEvaluator.h"

#include <memory>

namespace test_mock
{

using namespace geneial;
using ::geneial::population::chromosome::BaseChromosome;
using ::geneial::population::chromosome::MultiValueChromosome;

template<typename VALUE_TYPE, typename FITNESS_TYPE>
inline typename BaseChromosome<FITNESS_TYPE>::ptr baseCastMVC(
        typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr chromosome)
{
    typename BaseChromosome<FITNESS_TYPE>::ptr result = std::dynamic_pointer_cast < BaseChromosome<FITNESS_TYPE>
            > (chromosome);
    return result;
}

MultiValueChromosome<int, double>::const_ptr getMockIntMVC(std::vector<int> values,
        FitnessEvaluator<double>::ptr ptr_evaluator = std::shared_ptr<FitnessEvaluator<double> >())
{
    geneial::FitnessEvaluator<double>::ptr evaluator = ptr_evaluator;
    if (!evaluator)
    {
        evaluator = FitnessEvaluator<double>::ptr(new MockFitnessEvaluator<double>());
    }
    MultiValueChromosome<int, double>::ptr new_chromosome(new MultiValueChromosome<int, double>(evaluator));
    new_chromosome->getContainer().reserve(values.size());
    new_chromosome->getContainer().swap(values);
    return new_chromosome;
}

MultiValueChromosome<double, double>::const_ptr getMockDoubleMVC(std::vector<double> values,
        FitnessEvaluator<double>::ptr ptr_evaluator = std::shared_ptr<FitnessEvaluator<double> >())
{
    geneial::FitnessEvaluator<double>::ptr evaluator = ptr_evaluator;
    if (!evaluator)
    {
        evaluator = FitnessEvaluator<double>::ptr(new MockFitnessEvaluator<double>());
    }

    MultiValueChromosome<double, double>::ptr new_chromosome(new MultiValueChromosome<double, double>(evaluator));
    new_chromosome->getContainer().reserve(values.size());
    new_chromosome->getContainer().swap(values);
    return new_chromosome;
}

} /* namespace test_mock */

