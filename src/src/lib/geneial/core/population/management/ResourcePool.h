#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/builder/BaseChromosomeFactory.h>
#include <geneial/core/population/builder/MultiValueBuilderSettings.h>
#include <mutex>


geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(management)
{

geneial_export_namespace
{

/**
 * @brief Resource Management Class for Chromosomes.
 *
 * This class holds a pool of allocated memory.
 *
 * Note this class should be high performant and any method is
 * expected to be called frequently.
 *
 */
template<typename RESOURCE>
class ResourcePool
{
    std::mutex _poolMutex;

    std::vector<RESOURCE*> _freePool;
    //TODO (bewo): Think about pruning when having large amounts of resources.

public:

    /**
     * Takes a raw ptr to a resource managed by the pool.
     *
     * @return bool if the raw pointer was modified.
     * @arg raw_ptr the pointer to be modifed.
     */
    bool inline retrieve(RESOURCE* &raw_ptr)
    {
        std::lock_guard<std::mutex> lock(_poolMutex);
        if (_freePool.size() > 0)
        {
            raw_ptr = _freePool.back();
            _freePool.pop_back();
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
     * Free's the resource that was contained in raw_ptr.
     * @arg raw_ptr the raw ptr to free.
     */
    void inline free(RESOURCE*&& raw_ptr)
    {
#ifndef NALLOCREUSE
        std::lock_guard<std::mutex> lock(_poolMutex);
        _freePool.emplace_back(raw_ptr);
#else
        delete raw_ptr;
#endif
    }

    size_t inline size()
    {
        return _freePool.size();
    }

    virtual ~ResourcePool()
    {
        //The used pool is out in the wild should be guarded by some sharedptr.
        for (auto & ptr : _freePool)
        {
            delete ptr;
        }
    }
};


} /* geneial_export_namespace */
} /* private namespace management */
} /* private namespace population */
} /* private namespace geneial */
