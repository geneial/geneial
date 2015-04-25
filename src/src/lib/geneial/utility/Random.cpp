#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cstdlib> 		/* RAND_MAX */
#include <limits>       /* min, max of float, double */
#include <iostream>
#include <cassert>

#include <geneial/utility/Random.h>

namespace geneial
{
namespace utility
{

//Assures we have positive mod every time. (because in cpp -6 % 5 = -1)
inline int mymod(int k, int n)
{
    return ((k %= n) < 0) ? k + n : k;
}

Random::Random()
{
    srand(static_cast<unsigned>(time(0)));
}

bool Random::generateBit() const
{
    return rand() & 1;
}

bool Random::decision(const double probability) const
{
    assert(0.0 <= probability);
    assert(1.0 >= probability);
    const double reference = generateDouble(0.0, 1.0);
    if (probability == 0.0)
    {
        return false;
    }
    else
    {
        return (reference <= probability);
    }
}

int Random::generateInt() const
{
    return rand();
}

int Random::generateInt(const int min, const int max) const
{
    assert(min <= max);
    int sign = 1;
    if (min < 0)
    {
        sign *= -(rand() % 2);
    }
    const int result = mymod(sign * rand(), (max - min + 1)) + min;
    return result;
}

double Random::generateDouble(const double min, const double max) const
{
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

double Random::generateDouble() const
{
    return generateDouble(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
}

float Random::generateFloat(const float min, const float max) const
{
    assert(min <= max);
    const float range = (max - min);
    const float div = ((float) RAND_MAX) / range;
    const float random = min + (((float) rand()) / div);
    return random;
}

float Random::generateFloat() const
{
    return generateFloat(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
}

} /* namespace utility */
} /* namespace geneial */
