#pragma once

#include <geneial/utility/ThreadedExecutionManager.h>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(utility)
    {
        geneial_export_namespace
        {

            inline void ThreadedExecutionManager::executor()
            {
                for (;;)
                {
                    std::vector<std::function<void()>> batch;
                    {
                        std::unique_lock lock(_mutex);
                        _condEntry.wait(lock, [this] { return _finish || !_tasks.empty(); });
                        if (_tasks.empty() && _finish)
                        {
                            return;
                        }
                        while (!_tasks.empty() && batch.size() < _amountPerThread)
                        {
                            batch.emplace_back(std::move(_tasks.front()));
                            _tasks.pop_front();
                        }
                        _activeTasks += batch.size();
                    }
                    for (const auto &task : batch)
                    {
                        try
                        {
                            task();
                        }
                        catch (...)
                        {
                            std::lock_guard lock(_mutex);
                            if (!_exception)
                            {
                                _exception = std::current_exception();
                            }
                        }
                    }
                    {
                        std::lock_guard lock(_mutex);
                        _activeTasks -= batch.size();
                    }
                    _condExit.notify_all();
                }
            }

            inline void ThreadedExecutionManager::addTask(const std::function<void()> &task)
            {
                if (!task)
                {
                    throw std::invalid_argument("Task cannot be empty");
                }
                {
                    std::lock_guard lock(_mutex);
                    if (_finish)
                    {
                        throw std::logic_error("Cannot submit tasks after shutdown");
                    }
                    _tasks.emplace_back(task);
                }
                _condEntry.notify_one();
            }

            inline void ThreadedExecutionManager::waitForTasks()
            {
                std::unique_lock lock(_mutex);
                _condExit.wait(lock, [this] { return _tasks.empty() && _activeTasks == 0; });
                const auto failure = std::exchange(_exception, nullptr);
                lock.unlock();
                if (failure)
                {
                    std::rethrow_exception(failure);
                }
            }

            inline void ThreadedExecutionManager::joinAll()
            {
                {
                    std::lock_guard lock(_mutex);
                    _finish = true;
                }
                _condEntry.notify_all();
                for (auto &worker : _threads)
                {
                    if (worker.joinable())
                    {
                        worker.join();
                    }
                }
            }
        }
    }
}
