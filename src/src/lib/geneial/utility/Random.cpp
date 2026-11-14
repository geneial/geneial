#include <geneial/utility/Random.h>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(utility)
    {

        geneial_export_namespace
        {
            std::mt19937 &Random::engine()
            {
                static thread_local std::mt19937 generator(std::random_device{}());
                return generator;
            }

            void Random::initialize()
            {
                initialize(std::random_device{}());
            }

            void Random::initialize(std::uint32_t seed)
            {
                engine().seed(seed);
            }

        } /* geneial_export_namespace */
    } /* private namespace utility */
} /* private namespace geneial */
