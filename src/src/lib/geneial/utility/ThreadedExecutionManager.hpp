#pragma once

#include <geneial/utility/ThreadedExecutionManager.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

geneial_export_namespace
{
void ThreadedExecutionManager::executor()
{
    std::deque<std::function<void()>> innerTask;

    bool running = true;
    while (running)
    {
        std::unique_lock<std::mutex> l(_mutex);
        _condEntry.wait(l, [this]()
        {
            return _finish || _tasks.size() != 0;
        });

        if (!_tasks.empty())
        {
            int i = _amountPerThread;

            while (i-- && !_tasks.empty())
            {
                auto task = _tasks.front();
                _tasks.pop_front();
                innerTask.emplace_back(task);
            }
            const size_t taskToProcess = innerTask.size();
            _activeTasks += taskToProcess;
            l.unlock();

            size_t processedTasks = 0;
            for (auto task : innerTask)
            {
                processedTasks++;
                task();
            }
            innerTask.clear();
            assert(processedTasks == taskToProcess);
            l.lock();
            _activeTasks -= processedTasks;
        }
        else
        {
            running = !_finish;
        }

        _condExit.notify_all();
    }
}

void ThreadedExecutionManager::initializeThreads(const unsigned int amountThreads)
{
    for (unsigned int i = 0; i < amountThreads; ++i)
    {
        _threads.emplace_back(
                std::make_shared<std::thread>(&ThreadedExecutionManager::executor, this));
    }
}

void ThreadedExecutionManager::addTask(std::function<void()> const &task)
{
    std::unique_lock < std::mutex > l(_mutex);
    _tasks.emplace_back(task);
    _condEntry.notify_one();
}

void ThreadedExecutionManager::waitForTasks()
{
    unsigned int activeTasks = 0;
    do
    {
        std::unique_lock < std::mutex > l(_mutex);
        activeTasks = _tasks.size() + _activeTasks;
        if (activeTasks != 0)
        {
            _condExit.wait(l, [this]()
            {   return _tasks.size() == 0 && _activeTasks == 0;});
        }
    } while (activeTasks != 0);
}

void ThreadedExecutionManager::joinAll()
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

} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */
