#pragma once

#include <algorithm>
#include <cstddef>
#include <geneial/namespaces.h>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(population)
    {
        geneial_private_namespace(management)
        {
            geneial_export_namespace
            {

                /** Thread-safe, bounded ownership of idle resources. Outstanding resources remain caller-owned. */
                template <typename RESOURCE> class ResourcePool
                {
                    mutable std::mutex _poolMutex;
                    std::vector<RESOURCE *> _freePool;
                    std::size_t _capacity;

                  public:
                    static constexpr std::size_t DEFAULT_CAPACITY = 256;
                    explicit ResourcePool(std::size_t capacity = DEFAULT_CAPACITY) : _capacity(capacity) {}
                    ResourcePool(const ResourcePool &) = delete;
                    ResourcePool &operator=(const ResourcePool &) = delete;

                    bool retrieve(RESOURCE *&pointer)
                    {
                        std::lock_guard lock(_poolMutex);
                        if (_freePool.empty())
                            return false;
                        pointer = _freePool.back();
                        _freePool.pop_back();
                        return true;
                    }

                    /** Consumes the pointer; a full cache or failed cache allocation deletes the resource. */
                    void free(RESOURCE *&&pointer) noexcept
                    {
                        std::unique_ptr<RESOURCE> owned(std::exchange(pointer, nullptr));
#ifndef NALLOCREUSE
                        try
                        {
                            std::lock_guard lock(_poolMutex);
                            if (owned && _freePool.size() < _capacity)
                            {
                                _freePool.push_back(owned.get());
                                owned.release();
                            }
                        }
                        catch (...)
                        {
                        }
#endif
                    }

                    std::size_t size() const
                    {
                        std::lock_guard lock(_poolMutex);
                        return _freePool.size();
                    }

                    std::size_t capacity() const
                    {
                        std::lock_guard lock(_poolMutex);
                        return _capacity;
                    }

                    void setCapacity(std::size_t capacity)
                    {
                        std::vector<RESOURCE *> discarded;
                        {
                            std::lock_guard lock(_poolMutex);
                            const auto retained = std::min(capacity, _freePool.size());
                            discarded.assign(_freePool.begin() + retained, _freePool.end());
                            _freePool.resize(retained);
                            _capacity = capacity;
                        }
                        for (auto *resource : discarded)
                            delete resource;
                    }

                    void clear()
                    {
                        std::vector<RESOURCE *> discarded;
                        {
                            std::lock_guard lock(_poolMutex);
                            discarded.swap(_freePool);
                        }
                        for (auto *resource : discarded)
                            delete resource;
                    }

                    ~ResourcePool()
                    {
                        clear();
                    }
                };
            }
        }
    }
}
