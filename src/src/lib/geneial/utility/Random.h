#pragma once

#include <geneial/utility/patterns/singleton.hpp>

namespace geneial
{
namespace utility
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
} /* namespace utility */
} /* namespace geneial */

#include <geneial/utility/Random.hpp>
