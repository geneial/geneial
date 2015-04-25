#ifndef __GENEIAL_FITNESS_HPP_
#define __GENEIAL_FITNESS_HPP_
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

#endif /* __GENEIAL_FITNESS_HPP_ */
