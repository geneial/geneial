#pragma once

#include <geneial/core/operations/crossover/MultiValuePermutationCrossover.h>
#include <geneial/utility/Random.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(crossover)
{
using ::geneial::population::Population;
using ::geneial::population::chromosome::MultiValueChromosome;
using ::geneial::operation::coupling::BaseCouplingOperation;
using ::geneial::utility::Random;

enum COPY_MODE
{
	COPY_LEFT_PART,
	COPY_RIGHT_PART
};

template<typename VALUE_TYPE, typename FITNESS_TYPE>
void copyContainerPart(const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::value_container& source,
					   typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::value_container& dest,
					   COPY_MODE mode,
					   const unsigned int separator
)
{
	assert(separator < source.size());
	assert(separator >= 0);
	assert(separator < dest.size());

	if(mode == COPY_MODE::COPY_LEFT_PART)
	{
		std::copy(source.begin() , source.begin() + separator + 1, dest.begin());
	}
	else //RIGHT PART
	{
		std::copy(source.begin() + separator , source.end(), dest.begin() + separator);
	}
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
void copyRemainder(const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::value_container& source,
				   typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::value_container& dest,
				   COPY_MODE alreadyCopied,
				   const unsigned int separator)
{
	if(alreadyCopied == COPY_MODE::COPY_LEFT_PART)
	{
		//We need to copy everything to begin()  --> end() from source
		//Values already present are in begin() --> begin() + separator of dest
		unsigned int nextSourcePos = 0;
		for(unsigned int nextCopyPos = separator + 1; nextCopyPos < dest.size(); nextCopyPos ++)
		{
			//Find a value that we can copy.
			for(; nextSourcePos < source.size();nextSourcePos++)
			{
				//Do we have this value already?
				if(std::find( dest.begin(), dest.begin() + nextCopyPos, source[nextSourcePos]) != dest.begin() + nextCopyPos)
				{
					continue;
				}
				else
				{
					break;
				}
			}
			dest[nextCopyPos] = source[nextSourcePos];
			nextSourcePos++;
		}
	}
	else // Right Part was copied
	{
		//We need to copy everything to begin()  --> end() from source what is not contained in dest
		//Values already present are in begin() + separator --> end() separator of dest
		unsigned int nextSourcePos = 0;
		for(int nextCopyPos = separator -1 ; nextCopyPos >= 0; nextCopyPos --)
		{
			//Find a value that we can copy.
			for(; nextSourcePos < source.size();nextSourcePos++)
			{
				//Do we have this value already?
				if(std::find( dest.begin()+nextCopyPos + 1, dest.end(), source[nextSourcePos]) != dest.end())
				{
					continue;
				}
				else
				{
					break;
				}
			}
			dest[nextCopyPos] = source[nextSourcePos];
			nextSourcePos++;
		}
	}
}

geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseCrossoverOperation<FITNESS_TYPE>::crossover_result_set MultiValuePermutationCrossover<VALUE_TYPE,
        FITNESS_TYPE>::doMultiValueCrossover(
        const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &mommy,
        const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_ptr &daddy) const
{

    typename BaseCouplingOperation<FITNESS_TYPE>::offspring_result_set resultset;

    auto child_candidate = this->createChildCandidate();

    const auto &daddy_container = daddy->getContainer();
    const auto &mommy_container = mommy->getContainer();

    auto &child_container = child_candidate->getContainer();

    assert(daddy_container.size() == mommy_container.size());

    COPY_MODE copyMode = Random::generateBit() ? COPY_MODE::COPY_LEFT_PART : COPY_MODE::COPY_RIGHT_PART;
    auto startWithMommy = Random::generateBit();
    const unsigned int where = Random::generate<int>(0, daddy_container.size() -1 );

    child_container.resize(daddy_container.size(),0);


    copyContainerPart<VALUE_TYPE,FITNESS_TYPE>(startWithMommy?mommy_container:daddy_container, child_container, copyMode, where);
    copyRemainder<VALUE_TYPE,FITNESS_TYPE>(startWithMommy?daddy_container:mommy_container, child_container, copyMode, where);

    //Now some parts of the container have elements of mommy/daddy.
    //Next we need to fill up the gap with


    resultset.emplace_back(std::move(child_candidate));
    return std::move(resultset);
}

} /* geneial_export_namespace */
} /* private namespace crossover */
} /* private namespace operation */
} /* private namespace geneial */
