#pragma once
#include <geneial/namespaces.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

geneial_export_namespace
{


/**
 * @brief This class can be used widget style like.
 * Will allow to call make shared on protected ctors.
 */
template<typename CLIENT>
class EnableMakeShared
{

protected:
    /**
     * @brief creates a shared pointer using make shared improved version
     * @param ctors arguments list for CLIENT 
     * @return a shared pointer to the initialized object's ctor
     */
    template<typename ...ARG>
    std::shared_ptr<CLIENT> static makeShared(ARG&&...arg)
    {
        struct LocalEnableMakeShared: public CLIENT
        {
            LocalEnableMakeShared(ARG&&...arg) :
                    CLIENT(std::forward<ARG>(arg)...)
            {
            }
        };

        return std::make_shared<LocalEnableMakeShared>(std::forward<ARG>(arg)...);
    }
};

} /* export namespace */
} /* namespace utility */
} /* namespace geneial */
