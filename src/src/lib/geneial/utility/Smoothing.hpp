#pragma once

#include <geneial/utility/Smoothing.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{
using ::geneial::population::chromosome::MultiValueChromosome;
geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
void Smoothing::restoreSmoothness(typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr chromosome,
        VALUE_TYPE maxAbsElevation, VALUE_TYPE min, VALUE_TYPE max)
{
    VALUE_TYPE lastVal = *(chromosome->getContainer().begin());

    for (auto& it: chromosome->getContainer())
    {
        const VALUE_TYPE currentValue = it;

        if (std::abs(-lastVal) < maxAbsElevation)
        {
            //We are within the boundaries of the allowed elevation..
            //Not much to do here...
            lastVal = currentValue;
        }
        else
        {
            //We have exceeded the boundaries of the allowed elevation
            if (currentValue < lastVal)
            {
                //Case descending
                it = std::max(lastVal - maxAbsElevation, min);
                lastVal = it;
            }
            else
            {
                //Case ascending
                it = std::min(lastVal + maxAbsElevation, max);
                lastVal = it;
            }

        }
    }

}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
void Smoothing::peakAt(unsigned int pos, unsigned int epsLeft, unsigned int epsRight, VALUE_TYPE elevation,
        typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::ptr chromosome)
{
    const unsigned int chromSize = chromosome->getSize();

    assert(pos <= chromSize);

    assert(0 <= pos);

    //Look at all the value to the LEFT of pos
    //Determine target left position (avoid underflow)
    unsigned int leftEpsPos;
    if (pos < epsLeft)
    {
        leftEpsPos = 0;
    }
    else
    {
        leftEpsPos = pos - epsLeft;
    }

    for (unsigned int i = pos; i > leftEpsPos; i--)
    {
        //How many pct have we advanced to the left?
        const double pctElevated = ((double) (i - leftEpsPos)) / (double) epsLeft;
        const VALUE_TYPE toModify = (VALUE_TYPE) (((double) pctElevated) * elevation);
        chromosome->getContainer()[i] += toModify;
    }

    //Look at all the values to the RIGHT of pos
    //avoid overflow.
    unsigned int rightEpsPos;
    if (pos + epsRight > chromSize)
    {
        rightEpsPos = chromSize;
    }
    else
    {
        rightEpsPos = pos + epsRight;
    }

    for (unsigned int i = pos + 1; i < rightEpsPos; i++)
    {
        //How many pct have we advanced to the right?
        const double pctElevated = ((double) (rightEpsPos - i)) / (double) epsRight;
        const VALUE_TYPE toModify = (VALUE_TYPE) ((1.0 - (double) pctElevated) * elevation);
        chromosome->getContainer()[i] += toModify;
    }

}

} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */

