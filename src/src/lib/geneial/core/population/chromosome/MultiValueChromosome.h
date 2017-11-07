#pragma once

#include <geneial/utility/Debug.h>

#include <geneial/namespaces.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>

#include <cassert>
#include <vector>
#include <algorithm>
#include <numeric>
#include <memory>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(chromosome)
{

geneial_export_namespace
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

    explicit MultiValueChromosome(typename FitnessEvaluator<FITNESS_TYPE>::ptr fitnessEvaluator) :
            BaseChromosome<FITNESS_TYPE>(fitnessEvaluator), _container()
    {
        //static_assert(std::is_arithmetic<VALUE_TYPE>::value,"VALUE_TYPE is expected to be an arithmetic data type!");
        static_assert(std::is_arithmetic<FITNESS_TYPE>::value,"FITNESS_TYPE is expected to be an arithmetic data type!");
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
        using std::swap;
        swap(other._container,_container);
        swap(other._fitnessEvaluator,this->_fitnessEvaluator);
        swap(other._cacheValid,_cacheValid);
        swap(other._hashCache,_hashCache);
    }

    virtual ~MultiValueChromosome()
    {
    }

    bool equals(const BaseChromosome<FITNESS_TYPE> &chromosome) const override;

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

    virtual void doInvalidate() override
    {
        invalidateHashCache();
        _hashCache = 0;
        //_container.clear();
    }

    void inline invalidateHashCache() const;

private:
    value_container _container;

    mutable bool _cacheValid;

    mutable chromsome_hash _hashCache;

};

} /* geneial_export_namespace */
} /* private namespace chromosome */
} /* private namespace population */
} /* private namespace geneial */

#include <geneial/core/population/chromosome/MultiValueChromosome.hpp>
