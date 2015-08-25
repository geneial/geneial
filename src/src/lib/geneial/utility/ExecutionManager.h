#pragma once

#include <geneial/namespaces.h>

#include <functional>

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

geneial_export_namespace
{

/**
 * @brief The execution manager can takes care of processing arbitrary task.
 *
 * @see ThreadedExecutationManager
 *
 */
class BaseExecutionManager
{
public:
    virtual ~BaseExecutionManager(){}
    /**
     * Add another worker task
     */
    virtual void addTask(std::function<void()> const &task) = 0;
    /**
     * Block until all tasks are completed
     */
    virtual void waitForTasks() = 0;
};


/**
 * The sequential Execution manager immediately executes any given task and is blocking
 */
class SequentialExecutionManager: public BaseExecutionManager
{
public:
    virtual void addTask(std::function<void()> const &task) override
    {
        task();
    }

    virtual void waitForTasks() override
    {
        //Due to sequentialness we can immediatedly return
        return;
    }

};



} /* geneial_export_namespace */
} /* private namespace utility */
} /* private namespace geneial */
