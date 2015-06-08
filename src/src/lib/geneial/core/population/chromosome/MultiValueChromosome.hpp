#pragma once

#include <geneial/namespaces.h>

#include <geneial/core/population/chromosome/MultiValueChromosome.h>

#include <boost/functional/hash.hpp>

#include <iterator>
#include <algorithm>
#include <limits>


geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{

geneial_export_namespace
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
bool MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::equals(
        const BaseChromosome<FITNESS_TYPE> &chromosome) const
{
    return this->hashEquals(chromosome);
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename BaseChromosome<FITNESS_TYPE>::chromsome_hash MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getHash() const
{
#ifndef NMVCHASH_CACHE
    if(!hasCache())
    {
#endif
        //TODO (bewo): Introduce a mutex lock here for threading.
        _hashCache = boost::hash_range(_container.begin(), _container.end());
#ifndef NMVCHASH_CACHE
        _cacheValid = true;
        }
#endif
    typename BaseChromosome<FITNESS_TYPE>::chromsome_hash result = _hashCache;
    return result;
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
unsigned int MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getSize() const
{
    return _container.size();
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::value_container& MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getContainer()
{
    invalidateHashCache();
    return _container;
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
const typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::value_container& MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getContainer() const
{
    return _container;
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
bool inline MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::hasCache() const
{
    return _cacheValid;
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
void inline MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::invalidateHashCache() const
{
    _cacheValid = false;
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
void MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::setValueContainer(const value_container &container)
{
    invalidateHashCache();
    _container = container;
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
void MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::setValueContainer(value_container&& container)
{
    invalidateHashCache();
    std::swap(_container,container);
}


template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_it MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getConstIt() const
{
    return _container.begin();
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::it MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getIt() const
{
    invalidateHashCache();
    return _container.begin();
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::it MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getMax() const
{
    return std::max_element(_container.begin(), _container.end());
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
typename MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::it MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getMin() const
{
    return std::min_element(_container.begin(), _container.end());
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
VALUE_TYPE MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getAverage() const
{
    const VALUE_TYPE sum = std::accumulate(_container.begin(), _container.end(), 0);
    const unsigned int size = getSize();

    if (size == 0)
    {
        return 0;
    }
    else
    {
        assert(size > 0);
        return sum / size;
    }
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
VALUE_TYPE MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getRange() const
{
    MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_it max = getMax();
    MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::const_it min = getMin();
    assert(max >= min);
    return *max - *min;
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
VALUE_TYPE MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::getSum() const
{
    VALUE_TYPE sum = std::accumulate(_container.begin(), _container.end(), 0);
    return sum;
}

template<typename VALUE_TYPE, typename FITNESS_TYPE>
void MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE>::print(std::ostream& os) const
{
    os << "Fitness: ";
    if (this->hasFitness())
    {
        os << this->getFitness();
    }
    else
    {
        os << "N/A";
    }
    os << ", ";
    os << "Age: " << this->getAge() << ", ";

    os << "Hash: ";
    this->printHash(os);
    os << std::endl;

    os << "Values: " << std::endl;
    std::ostream_iterator<VALUE_TYPE> out_it(os, "; ");
    std::copy(_container.begin(), _container.end(), out_it);
    os << std::endl;
}

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */

