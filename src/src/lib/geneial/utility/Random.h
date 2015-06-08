#pragma once

#include <geneial/namespaces.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

geneial_export_namespace
{

class Random
{
    Random() = delete;
    Random(const Random&) = delete;
public:
    static inline bool generateBit();

    static inline bool decision(const double probability);

    template<typename T>
    static inline T generate(const T min,const T max);

    template<typename T>
    static inline T generate();

    virtual ~Random()
    {
    }

    void static initialize();
protected:
    static bool sInitialized;

    static inline void checkInitialized()
    {
        if (!sInitialized)
        {
            initialize();
        }
    }

};

} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */

#include <geneial/utility/Random.hpp>
