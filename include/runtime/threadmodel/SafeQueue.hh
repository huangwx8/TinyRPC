#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

// A threadsafe-queue.
template <class T>
class SafeQueue
{
public:
    SafeQueue(std::size_t MaxSize) : q(), m(), c(), maxsize(MaxSize)
    {
    }

    ~SafeQueue(void)
    {
    }

    /** Add an element to the queue, with perfect forwarding */
    bool Enqueue(T&& t)
    {
        std::lock_guard<std::mutex> lock(m);

        if (q.size() >= maxsize)
        {
            return false;
        }

        q.push(t);
        c.notify_one();

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
        while (q.empty())
        {
            // release lock as long as the wait and reaquire it afterwards.
            if (block)
            {
                c.wait(lock);
            }
            else
            {
                return false;
            }
        }
        data = q.front();
        q.pop();
        return true;
    }

private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
    std::size_t maxsize;
};
