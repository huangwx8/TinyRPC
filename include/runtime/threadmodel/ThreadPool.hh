#pragma once

// std c++
#include <functional>
#include <thread>
#include <vector>

// inner
#include <runtime/threadmodel/SafeQueue.hh>

class ThreadPool
{
public:
    explicit ThreadPool(
        std::size_t thread_num = 1,
        std::size_t task_queue_size = 10000);

    ThreadPool() = delete;

    ~ThreadPool();

    bool AddTask(std::function<void()> &&job);

    void Stop();

    void Join();

    bool IsStop();

private:
    void Run();

private:
    std::vector<std::thread> threads_;

    SafeQueue<std::function<void()>> tasks_;

    bool m_stop;
};
