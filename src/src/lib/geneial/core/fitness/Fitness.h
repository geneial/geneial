#ifndef __GENEIAL_FITNESS_H_
#define __GENEIAL_FITNESS_H_

#include <geneial/utility/Printable.h>

#include <boost/shared_ptr.hpp>

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
    typedef boost::shared_ptr<Fitness<FITNESS_TYPE> > ptr;

    typedef boost::shared_ptr<const Fitness<FITNESS_TYPE> > const_ptr;

    Fitness()
    {
    }

    Fitness(FITNESS_TYPE value) :
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

#endif /* __GENEIAL_FITNESS_H_ */
