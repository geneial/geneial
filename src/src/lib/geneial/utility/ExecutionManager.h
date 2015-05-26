#pragma once

#include <geneial/namespaces.h>

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


//TODO (bewo) refactor, separate def from decl to .hpp

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

geneial_export_namespace
{

class BaseExecutionManager
{
public:
    virtual ~BaseExecutionManager(){}
    virtual void addTask(std::function<void()> const &task) = 0;
    virtual void waitForTasks() = 0;
};

class SequentialExecutionManager: public BaseExecutionManager
{
public:
    virtual void addTask(std::function<void()> const &task) override
    {
        task();
    }

    virtual void waitForTasks() override
    {
        return;
    }

};

class ThreadedExecutionManager: public BaseExecutionManager
{
    std::deque<std::function<void()>> _tasks;

    std::vector<std::shared_ptr<std::thread>> _threads;

    std::mutex _mutex;

    std::condition_variable _condEntry;

    std::condition_variable _condExit;

    bool _finish;

    void executor()
    {
        bool running = true;
        while (running)
        {
            std::unique_lock < std::mutex > l(_mutex);
            _condEntry.wait(l, [this]()
            {
                return _finish || _tasks.size() != 0;
            });
            if (!_tasks.empty())
            {
                auto task = _tasks.front();
                _tasks.pop_front();
                l.unlock();
                task();
            }
            else
            {
                running = !_finish;
            }
            _condExit.notify_all();
        }
    }

    void initializeThreads(const unsigned int amountThreads)
    {
        for (unsigned int i = 0; i < amountThreads; ++i)
        {
            _threads.push_back(
                    std::shared_ptr < std::thread > (new std::thread(&ThreadedExecutionManager::executor, this)));
        }
    }

public:

    explicit ThreadedExecutionManager(const unsigned int amountThreads) :
            _tasks(), _threads(), _mutex(), _condEntry(), _finish(false)
    {
        initializeThreads(amountThreads);
    }

    ThreadedExecutionManager() :
            _tasks(), _threads(), _mutex(), _condEntry(), _finish(false)
    {
        initializeThreads(
                std::min(static_cast<unsigned int>(1),
                        static_cast<unsigned int>(std::thread::hardware_concurrency() - 1)));
    }

    virtual ~ThreadedExecutionManager()
    {
        joinAll();
    }

    virtual void addTask(std::function<void()> const &task) override
    {
        std::unique_lock < std::mutex > l(_mutex);
        _tasks.push_back(task);
        _condEntry.notify_one();
    }

    virtual void waitForTasks() override
    {
        unsigned int activeTasks = 0;
        do
        {
            std::unique_lock < std::mutex > l(_mutex);
            activeTasks = _tasks.size();
            if (activeTasks != 0)
            {
                _condExit.wait(l, [this]()
                {   return _tasks.size() == 0;});
            }
        } while (activeTasks != 0);
    }

    void joinAll()
    {
        {
            std::unique_lock < std::mutex > l(_mutex);
            _finish = true;
        }
        _condEntry.notify_all();
        for (auto t : _threads)
        {
            t->join();
        }
    }
};

} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */

