#pragma once

/**
 *
 * @brief This file declares all geneial public namespaces and
 * defines shadow noexport namespace imports
 *
 * The idea is that geneial has public and private namespaces.
 * By default, everything is declared in private namespaces
 * to avoid namespace pollution when a geneial header is used.
 *
 * If a class, type, etc. needs to be made publicy available
 * it should be defined within the export inline namespace.
 *
 *
 * This file contains an enumeration of all namespaces being
 * used within geneial, new namespaces should be declared here accordingly.
 *
 */

#define geneial_namespace_noexport(NAME) __ ## NAME ##_
#define geneial_namespace_export _
#define geneial_public_namespace_name(NAME) NAME

#define geneial_private_namespace(NAME) namespace geneial_namespace_noexport(NAME)
#define geneial_public_namespace(NAME) namespace geneial_public_namespace_name(NAME)
#define geneial_export_namespace inline namespace geneial_namespace_export

/**
 * Define global Namespaces:
 */
geneial_public_namespace(geneial)
{
    geneial_public_namespace(algorithm)
    {
        geneial_public_namespace(stopping_criteria)
        {
        }
    }

    geneial_public_namespace(utility)
    {
    }

    geneial_public_namespace(population)
    {
        geneial_public_namespace(management)
        {
        }
        geneial_public_namespace(chromosome)
        {
        }
    }

    geneial_public_namespace(operation)
    {
        geneial_public_namespace(choosing)
        {
        }
        geneial_public_namespace(coupling)
        {
        }
        geneial_public_namespace(crossover)
        {
        }
        geneial_public_namespace(mutation)
        {
        }
        geneial_public_namespace(replacement)
        {
        }
        geneial_public_namespace(selection)
        {
        }
    }
}


/**
 * Define shadow namespaces
 */
geneial_private_namespace(geneial)
{
    geneial_export_namespace
    {
        using namespace ::geneial_public_namespace_name(geneial);
    }

    geneial_private_namespace(algorithm)
    {
        geneial_export_namespace
        {
            using namespace ::geneial_public_namespace_name(geneial)::geneial_public_namespace_name(algorithm);
        }

        geneial_private_namespace(stopping_criteria)
        {
            geneial_export_namespace
            {
                using namespace ::geneial_public_namespace_name(geneial)::geneial_public_namespace_name(algorithm)::geneial_public_namespace_name(stopping_criteria);
            }
        }
    }

    geneial_private_namespace(utility)
    {
        geneial_export_namespace
        {
            using namespace ::geneial_public_namespace_name(geneial)::geneial_public_namespace_name(utility);
        }
    }

    geneial_private_namespace(population)
    {
        geneial_export_namespace
        {
            using namespace ::geneial_public_namespace_name(geneial)::geneial_public_namespace_name(population);
        }

        geneial_private_namespace(management)
        {
            geneial_export_namespace
            {
                using namespace ::geneial_public_namespace_name(geneial)::geneial_public_namespace_name(population)::geneial_public_namespace_name(management);
            }
        }

        geneial_private_namespace(chromosome)
        {
            geneial_export_namespace
            {
                using namespace ::geneial_public_namespace_name(geneial)::geneial_public_namespace_name(population)::geneial_public_namespace_name(chromosome);
            }
        }
    }

    geneial_private_namespace(operation)
    {
        geneial_export_namespace
        {
            using namespace ::geneial_public_namespace_name(geneial)::geneial_public_namespace_name(operation);
        }

        geneial_private_namespace(choosing)
        {
            geneial_export_namespace
            {
                using namespace ::geneial_public_namespace_name(geneial)::geneial_public_namespace_name(operation)::geneial_public_namespace_name(choosing);
            }
        }

        geneial_private_namespace(coupling)
        {
            geneial_export_namespace
            {
                using namespace ::geneial_public_namespace_name(geneial)::geneial_public_namespace_name(operation)::geneial_public_namespace_name(coupling);
            }
        }

        geneial_private_namespace(crossover)
        {
            geneial_export_namespace
            {
                using namespace ::geneial_public_namespace_name(geneial)::geneial_public_namespace_name(operation)::geneial_public_namespace_name(crossover);
            }
        }

        geneial_private_namespace(mutation)
        {
            geneial_export_namespace
            {
                using namespace ::geneial_public_namespace_name(geneial)::geneial_public_namespace_name(operation)::geneial_public_namespace_name(mutation);
            }
        }

        geneial_private_namespace(replacement)
        {
            geneial_export_namespace
            {
                using namespace ::geneial_public_namespace_name(geneial)::geneial_public_namespace_name(operation)::geneial_public_namespace_name(replacement);
            }
        }

        geneial_private_namespace(selection)
        {
            geneial_export_namespace
            {
                using namespace ::geneial_public_namespace_name(geneial)::geneial_public_namespace_name(operation)::geneial_public_namespace_name(selection);
            }
        }

    }

}

/**
 * Global namespace imports shadow namespaces
 */
geneial_public_namespace(geneial)
{
    using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_export;

    geneial_public_namespace(algorithm)
    {
        using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_noexport(algorithm)::geneial_namespace_export;

        geneial_public_namespace(stopping_criteria)
        {
            using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_noexport(algorithm)::geneial_namespace_noexport(stopping_criteria)::geneial_namespace_export;
        }
    }

    geneial_public_namespace(utility)
    {
        using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_noexport(utility)::geneial_namespace_export;
    }

    geneial_public_namespace(population)
    {
        using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_noexport(population)::geneial_namespace_export;

        geneial_public_namespace(management)
        {
            using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_noexport(population)::geneial_namespace_noexport(management)::geneial_namespace_export;
        }

        geneial_public_namespace(chromosome)
        {
            using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_noexport(population)::geneial_namespace_noexport(chromosome)::geneial_namespace_export;
        }
    }

    geneial_public_namespace(operation)
    {
        using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_noexport(operation)::geneial_namespace_export;

        geneial_public_namespace(choosing)
        {
            using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_noexport(operation)::geneial_namespace_noexport(choosing)::geneial_namespace_export;
        }

        geneial_public_namespace(coupling)
        {
            using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_noexport(operation)::geneial_namespace_noexport(coupling)::geneial_namespace_export;
        }

        geneial_public_namespace(crossover)
        {
            using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_noexport(operation)::geneial_namespace_noexport(crossover)::geneial_namespace_export;
        }

        geneial_public_namespace(mutation)
        {
            using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_noexport(operation)::geneial_namespace_noexport(mutation)::geneial_namespace_export;
        }

        geneial_public_namespace(replacement)
        {
            using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_noexport(operation)::geneial_namespace_noexport(replacement)::geneial_namespace_export;
        }

        geneial_public_namespace(selection)
        {
            using namespace ::geneial_namespace_noexport(geneial)::geneial_namespace_noexport(operation)::geneial_namespace_noexport(selection)::geneial_namespace_export;
        }
    }
}
