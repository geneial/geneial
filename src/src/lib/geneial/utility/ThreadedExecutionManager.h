#pragma once

#include <geneial/utility/ExecutionManager.h>

#include <iostream>
#include <deque>
#include <memory>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <list>

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

geneial_export_namespace
{

class ThreadedExecutionManager: public BaseExecutionManager
{
private:
    std::deque<std::function<void()>> _tasks;

    int _activeTasks = 0;

    std::vector<std::shared_ptr<std::thread>> _threads;

    std::mutex _mutex;

    std::condition_variable _condEntry;

    std::condition_variable _condExit;

    bool _finish;

    void inline executor();

    void inline initializeThreads(const unsigned int amountThreads);

    unsigned int _amountPerThread = 1;

public:

    explicit ThreadedExecutionManager(const unsigned int amountThreads) :
            _tasks(), _threads(), _mutex(), _condEntry(), _finish(false),_amountPerThread(1)
    {
        initializeThreads(amountThreads);
    }

    ThreadedExecutionManager() :
            _tasks(), _threads(), _mutex(), _condEntry(), _finish(false),_amountPerThread(1)
    {
        initializeThreads(
                std::max(static_cast<unsigned int>(1),
                        static_cast<unsigned int>(std::thread::hardware_concurrency() - 1)));
    }

    virtual ~ThreadedExecutionManager()
    {
        joinAll();
    }

    virtual inline void addTask(std::function<void()> const &task) override;

    virtual inline void waitForTasks() override;

    void inline joinAll();

    unsigned int getAmountPerThread() const
    {
        return _amountPerThread;
    }

    /**
     * Determines the work bucket size for one thread,
     * i.e. how much work does one thread do until the thread syncs again.
     */
    void setAmountPerThread(unsigned int amountPerThread = 1)
    {
        _amountPerThread = amountPerThread;
    }
};

} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */

#include <geneial/utility/ThreadedExecutionManager.hpp>
