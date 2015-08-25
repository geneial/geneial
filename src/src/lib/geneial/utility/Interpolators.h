#pragma once

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

geneial_export_namespace
{

//Library for some common smoothing operations


class Interpolators{

private:
    Interpolators(){};
    virtual ~Interpolators(){};

public:

    template<typename VALUE_TYPE>
    static inline VALUE_TYPE linearInterpolate(VALUE_TYPE y0, VALUE_TYPE x0, VALUE_TYPE y1, VALUE_TYPE x1, VALUE_TYPE x);

    template<typename VALUE_TYPE>
    static inline VALUE_TYPE linearInterpolate(VALUE_TYPE y0, VALUE_TYPE y1, VALUE_TYPE t);



    template<typename VALUE_TYPE>
    static inline VALUE_TYPE cosineInterpolate(VALUE_TYPE y0, VALUE_TYPE x0, VALUE_TYPE y1, VALUE_TYPE x1, VALUE_TYPE x);

    template<typename VALUE_TYPE>
    static inline VALUE_TYPE cosineInterpolate(VALUE_TYPE y0, VALUE_TYPE y1, VALUE_TYPE t);

};

} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */



#include <geneial/utility/Interpolators.hpp>


