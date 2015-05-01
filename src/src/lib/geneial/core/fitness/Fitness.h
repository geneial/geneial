#pragma once

#include <geneial/utility/Printable.h>

#include <memory>

namespace geneial
{

using namespace geneial::utility;

template<typename FITNESS_TYPE>
class Fitness: public Printable
{
private:
    FITNESS_TYPE _value;

public:
    //TODO (CODECONSISTENCY) (bewo): boost sharedptr shared from this?
    typedef std::shared_ptr<Fitness<FITNESS_TYPE> > ptr;

    typedef std::shared_ptr<const Fitness<FITNESS_TYPE> > const_ptr;

    Fitness()
    {
    }

    explicit Fitness(FITNESS_TYPE value) :
            _value(value)
    {
    }

    virtual ~Fitness()
    {
    }

    void set(FITNESS_TYPE value);

    FITNESS_TYPE get() const;

    //cast operator overload
    operator FITNESS_TYPE() const
    {
        return get();
    }

    void print(std::ostream&) const;
};

} /* namespace geneial */

#include <geneial/core/fitness/Fitness.hpp>

