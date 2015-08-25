#pragma once

#include <geneial/namespaces.h>

#include <cmath>
#include <cassert>

#ifndef M_PI
//TODO (bewo) move to const double..
#define M_PI    3.14159265358979323846f
#endif

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

geneial_export_namespace
{

template<typename VALUE_TYPE>
VALUE_TYPE Interpolators::linearInterpolate(VALUE_TYPE y0, VALUE_TYPE x0, VALUE_TYPE y1, VALUE_TYPE x1, VALUE_TYPE x)
{
    if ((x0 - x1) <= 1e-7)
    {
        assert(std::abs(y1 - y0) <= 1e-7);
        return y1;
    }

    if (x1 >= x0)
    {
        return linearInterpolate<VALUE_TYPE>(y0, y1, (x - x0) / (x1 - x0));
    }
    else
    {

        return linearInterpolate<VALUE_TYPE>(y1, y0, (x - x1) / (x0 - x1));
    }
}

template<typename VALUE_TYPE>
VALUE_TYPE Interpolators::linearInterpolate(VALUE_TYPE y0, VALUE_TYPE y1, VALUE_TYPE t)
{
    return (1 - t) * y0 + t * y1;
}

template<typename VALUE_TYPE>
VALUE_TYPE Interpolators::cosineInterpolate(VALUE_TYPE y0, VALUE_TYPE x0, VALUE_TYPE y1, VALUE_TYPE x1, VALUE_TYPE x)
{
    if ((x0 - x1) <= 1e-7)
    {
        assert(std::abs(y1 - y0) <= 1e-7);
        return y1;
    }

    if (x1 >= x0)
    {
        return linearInterpolate<VALUE_TYPE>(y0, y1, (x - x0) / (x1 - x0));
    }
    else
    {

        return linearInterpolate<VALUE_TYPE>(y1, y0, (x - x1) / (x0 - x1));
    }
}

template<typename VALUE_TYPE>
VALUE_TYPE Interpolators::cosineInterpolate(VALUE_TYPE y0, VALUE_TYPE y1, VALUE_TYPE t)
{
    const double t2 = (1 - std::cos(t * M_PI)) / 2;
    return (y0 * (1 - t2) + y1 * t2);
}

} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */

