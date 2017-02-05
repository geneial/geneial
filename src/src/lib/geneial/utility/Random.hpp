#pragma once

#include <geneial/utility/Random.h>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cstdlib>         /* RAND_MAX */
#include <limits>       /* min, max of float, double */
#include <random>
#include <iostream>
#include <cassert>



//TODO (bewo) benchmark and possibly use new c++11 random functions

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

//Assures we have positive mod every time. (because in cpp -6 % 5 = -1)
inline int mymod(int k, const int n)
{
    return ((k %= n) < 0) ? k + n : k;
}

geneial_export_namespace
{

template<>
inline int Random::generate(const int min, const int max)
{
    checkInitialized();
    assert(min <= max);
    int sign = 1;
    if (min < 0)
    {
        sign *= -(rand() % 2);
    }
    const int result = mymod(sign * rand(), (max - min + 1)) + min;
    return result;
}



template<>
inline unsigned int Random::generate(const unsigned int min, const unsigned int max)
{
    checkInitialized();
    assert(min <= max);
    const unsigned int result = mymod(rand(), (max - min + 1)) + min;
    return result;
}

template<>
inline int Random::generate()
{
    checkInitialized();
    return rand();
}

template<>
inline double Random::generate(const double min, const double max)
{
    checkInitialized();
    assert(min <= max);
    int sign = 1;
    if (min < 0)
    {
        sign *= -(rand() % 2);
    }
    const double range = (max - min);
    const double div = RAND_MAX / range;
    const double random = min + (rand() / div);
    return random;
}



template<>
inline double Random::generate()
{
    checkInitialized();
    return Random::generate<double>(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
}

template<>
inline float Random::generate(const float min, const float max)
{
    checkInitialized();
    assert(min <= max);
    const float range = (max - min);
    const float div = ((float) RAND_MAX) / range;
    const float random = min + (((float) rand()) / div);
    return random;
}

template<>
inline float Random::generate()
{
    checkInitialized();
    return Random::generate(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());

}

bool Random::generateBit()
{
    checkInitialized();
    return rand() & 1;
}

bool Random::decision(const double probability)
{
    checkInitialized();
    assert(0.0 <= probability);
    assert(1.0 >= probability);
    const double reference = generate<double>(0.0, 1.0);
    if (probability == 0.0)
    {
        return false;
    }
    else
    {
    	return reference <= probability;
    }
}

inline void Random::initialize()
{
    srand(static_cast<unsigned>(time(0)));
    sInitialized = true;
}
} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */
