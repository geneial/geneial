#pragma once

#include <geneial/core/population/chromosome/BaseChromosome.h>

#include <cassert>
#include <vector>
#include <algorithm>
#include <numeric>
#include <memory>

namespace geneial
{
namespace population
{
namespace chromosome
{

template<typename VALUE_TYPE, typename FITNESS_TYPE>
class MultiValueChromosome: public BaseChromosome<FITNESS_TYPE>
{
public:

    typedef typename BaseChromosome<FITNESS_TYPE>::chromsome_hash chromsome_hash;

    typedef typename std::vector<VALUE_TYPE> value_container;
    typedef typename value_container::const_iterator const_it;
    typedef typename value_container::iterator it;

    typedef typename std::shared_ptr<MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> > ptr;
    typedef typename std::shared_ptr<const MultiValueChromosome<VALUE_TYPE, FITNESS_TYPE> > const_ptr;

    MultiValueChromosome(typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator) :
                BaseChromosome<FITNESS_TYPE>(fitnessEvaluator), _container()
        {
        }


    /**
     * Copy Constructor
     * @param other the other ctor to copy from
     */
    MultiValueChromosome(const MultiValueChromosome& other) :
            BaseChromosome<FITNESS_TYPE>(other._fitnessEvaluator), _container(other._container),
            _cacheValid(other._cacheValid), _hashCache(other._hashCache)
    {
    }

    /**
     * RValue Ctor
     * @param other
     */
    MultiValueChromosome(MultiValueChromosome&& other) :
            BaseChromosome<FITNESS_TYPE>(other._fitnessEvaluator), _container(),
            _cacheValid(other._cacheValid), _hashCache(other._hashCache)
    {
        swap(other);
    }

    /**
     * Swaps the contents of the other Multi Value Chromosome
     * @param other
     */
    void swap(MultiValueChromosome& other)
    {
        std::swap(other._container,_container);
        std::swap(other._fitnessEvaluator,this->_fitnessEvaluator);
        std::swap(other._cacheValid,_cacheValid);
        std::swap(other._hashCache,_hashCache);
    }

    virtual ~MultiValueChromosome()
    {
    }

    bool equals(typename BaseChromosome<FITNESS_TYPE>::const_ptr chromosome) const override;

    const_it getConstIt() const;
    it getIt() const;

    unsigned int getSize() const;

    it getMax() const;
    it getMin() const;

    VALUE_TYPE getAverage() const;
    VALUE_TYPE getRange() const;
    VALUE_TYPE getSum() const;

    //TODO (bewo) : provide further convenience methods at this point.

    value_container& getContainer();

    const value_container& getContainer() const;

    void setValueContainer(const value_container& container);

    void setValueContainer(value_container&& container);



    void print(std::ostream& os) const override;

    chromsome_hash getHash() const override;

    bool inline hasCache() const;

    void inline invalidateHashCache();

private:
    value_container _container;

    mutable bool _cacheValid;

    mutable chromsome_hash _hashCache;

};

} /* namespace chromomsome */
} /* namespace population */
} /* namespace geneial */

#include <geneial/core/population/chromosome/MultiValueChromosome.hpp>
