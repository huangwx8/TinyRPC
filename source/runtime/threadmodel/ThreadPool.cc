// self
#include <runtime/threadmodel/ThreadPool.hh>

// c
#include <assert.h>
#include <utility>

ThreadPool::ThreadPool(
    std::size_t thread_num = 1,
    std::size_t task_queue_size = 10000)
{
    tasks_(task_queue_size);

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
}

bool ThreadPool::AddTask(std::function<void()> &&job)
{

}

void ThreadPool::Join()
{
    for (auto &&t : threads_)
    {
        t.join();
    }
}

void ThreadPool::Run()
{

}
