// self
#include <runtime/threadmodel/ThreadPool.hh>

// c
#include <assert.h>
#include <utility>

ThreadPool::ThreadPool(
    std::size_t thread_num,
    std::size_t task_queue_size):
    tasks_(task_queue_size),
    m_stop(false)
{
    assert(thread_num > 0);
    assert(task_queue_size > 0);
    
    threads_.resize(thread_num);

    for (auto &&t : threads_)
    {
        t = std::thread([this]
        {
            this->Run();
        });
    }
}

ThreadPool::~ThreadPool()
{
    Stop();
}

bool ThreadPool::AddTask(std::function<void()> &&job)
{
    return tasks_.Enqueue(std::forward<std::function<void()>>(job));
}

void ThreadPool::Stop()
{
    // Not a good implementation, considering to have a notify-mechanism over threads
    m_stop = true;
}

void ThreadPool::Join()
{
    // wait all child thread to terminate
    for (auto &&t : threads_)
    {
        t.join();
    }
}

void ThreadPool::Run()
{
    // Get task, do task, iteratively.
    while (!m_stop)
    {
        std::function<void()> task;
        if (tasks_.Dequeue(task, true))
        {
            task();
        }
        else
        {

        }
    }
}
