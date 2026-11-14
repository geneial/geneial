#pragma once

#include <geneial/utility/ExecutionManager.h>

#include <algorithm>
#include <condition_variable>
#include <deque>
#include <exception>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(utility)
    {
        geneial_export_namespace
        {

            class ThreadedExecutionManager : public BaseExecutionManager
            {
              private:
                std::deque<std::function<void()>> _tasks;
                std::size_t _activeTasks = 0;
                std::vector<std::jthread> _threads;
                mutable std::mutex _mutex;
                std::condition_variable _condEntry;
                std::condition_variable _condExit;
                bool _finish = false;
                unsigned int _amountPerThread = 1;
                std::exception_ptr _exception;

                void executor();

              public:
                explicit ThreadedExecutionManager(unsigned int amountThreads)
                {
                    if (amountThreads == 0)
                    {
                        throw std::invalid_argument("Thread count must be positive");
                    }
                    try
                    {
                        for (unsigned int index = 0; index < amountThreads; ++index)
                        {
                            _threads.emplace_back([this] { executor(); });
                        }
                    }
                    catch (...)
                    {
                        joinAll();
                        throw;
                    }
                }

                ThreadedExecutionManager()
                    : ThreadedExecutionManager(
                          std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1)
                {
                }

                ~ThreadedExecutionManager() override
                {
                    joinAll();
                }

                void addTask(const std::function<void()> &task) override;
                void waitForTasks() override;
                void joinAll();

                unsigned int getAmountPerThread() const
                {
                    std::lock_guard lock(_mutex);
                    return _amountPerThread;
                }

                void setAmountPerThread(unsigned int amountPerThread = 1)
                {
                    if (amountPerThread == 0)
                    {
                        throw std::invalid_argument("Task batch size must be positive");
                    }
                    std::lock_guard lock(_mutex);
                    _amountPerThread = amountPerThread;
                }
            };
        }
    }
}

#include <geneial/utility/ThreadedExecutionManager.hpp>
