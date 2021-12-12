#pragma once

#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "stdio.h"
#include <iostream>

// A threadsafe-queue.
template <class T>
class SafeQueue
{
public:
    SafeQueue(std::size_t MaxSize) : m(), c(), m_stop(false), q(), maxsize(MaxSize)
    {
    }

    ~SafeQueue()
    {
        m_stop = true;
        c.notify_all();

        // Make sure no thread is visiting this queue
        {
            std::unique_lock<std::mutex> lock(m);
        }
    }

    /** Add an element to the queue, with perfect forwarding */
    bool Enqueue(T&& t)
    {
        std::unique_lock<std::mutex> lock(m);

        std::cout << "SafeQueue::Enqueue: start enqueue from thread [" << std::this_thread::get_id() << "]\n";
        if (q.size() >= maxsize)
        {
            return false;
        }

        q.push(t);
        c.notify_one();

        std::cout << "SafeQueue::Enqueue: notify_one() from thread [" << std::this_thread::get_id() << "]\n";

        return true;
    }

    /**
     * @brief Get the "front"-element. 
     * 
     * @param data Reference to a out object data buffer
     * @param block If the queue is empty, whether to block till a element is avaiable.
     * @return boolean
     */
    bool Dequeue(T& data, bool block = false)
    {
        std::unique_lock<std::mutex> lock(m);
        if (q.empty())
        {
            if (block)
            {
                // release lock as long as
                // 1. some thread add a new task
                // 2. this queue is going to be destructed.
                std::cout << "SafeQueue::Dequeue: start wait() from thread [" << std::this_thread::get_id() << "]\n";
                c.wait(lock);
                std::cout << "SafeQueue::Dequeue: stop wait() from thread [" << std::this_thread::get_id() << "]\n";
            }
            else
            {
                return false;
            }
        }
        // since wait() can be interrupted by destructor, we need to branch here
        if (q.empty())
        {
            return false;
        }
        else
        {
            data = q.front();
            q.pop();
        }
        return true;
    }

private:
    // concurrency
    mutable std::mutex m;
    std::condition_variable c;
    std::atomic<bool> m_stop;
    // main queue data
    std::queue<T> q;
    std::size_t maxsize;
};
