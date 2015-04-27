#pragma once
#include <geneial/core/fitness/Fitness.h>

namespace geneial
{

template<typename FITNESS_TYPE>
FITNESS_TYPE Fitness<FITNESS_TYPE>::get() const
{
    return _value;
}

template<typename FITNESS_TYPE>
void Fitness<FITNESS_TYPE>::set(FITNESS_TYPE value)
{
    _value = value;
}

template<typename FITNESS_TYPE>
void Fitness<FITNESS_TYPE>::print(std::ostream& os) const
{
    os << _value;
}

} /* namespace geneial */

