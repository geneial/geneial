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

    std::vector<std::shared_ptr<std::thread>> _threads;

    std::mutex _mutex;

    std::condition_variable _condEntry;

    std::condition_variable _condExit;

    bool _finish;

    void executor();

    void initializeThreads(const unsigned int amountThreads);

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

    virtual void addTask(std::function<void()> const &task) override;

    virtual void waitForTasks() override;

    void joinAll();
};

} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */
