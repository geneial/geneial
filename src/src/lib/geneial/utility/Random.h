#pragma once

#include <geneial/namespaces.h>
#include <geneial/utility/patterns/Singleton.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

geneial_export_namespace
{

class Random
{
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
    static inline void checkInitialized();
    Random(){}

};
bool Random::sInitialized = false;

} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */

#include <geneial/utility/Random.hpp>
