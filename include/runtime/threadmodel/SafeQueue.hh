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

// Add an element to the queue.
bool Enqueue(T t)
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

// Get the "front"-element.
// If the queue is empty, wait till a element is avaiable.
bool Dequeue(T&& data, bool block = false)
{
    std::unique_lock<std::mutex> lock(m);
    while (q.empty())
    {
        // release lock as long as the wait and reaquire it afterwards.
        c.wait(lock);
    }
    T val = q.front();
    q.pop();
    return val;
}

private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
    std::size_t maxsize;
};
