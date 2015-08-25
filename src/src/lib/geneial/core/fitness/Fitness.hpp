#pragma once
#include <geneial/core/fitness/Fitness.h>

namespace __geneial_noexport
{
inline namespace exports
{

template<typename FITNESS_TYPE>
FITNESS_TYPE Fitness<FITNESS_TYPE>::get() const
{
    return *_value;
}

template<typename FITNESS_TYPE>
void Fitness<FITNESS_TYPE>::set(FITNESS_TYPE value)
{
    _value = value;
}

template<typename FITNESS_TYPE>
bool Fitness<FITNESS_TYPE>::isValid() const
{
    if(_value)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template<typename FITNESS_TYPE>
void Fitness<FITNESS_TYPE>::print(std::ostream& os) const
{
    os << *_value;
}

} /* namespace exports */
} /* namespace __geneial_noexport */
