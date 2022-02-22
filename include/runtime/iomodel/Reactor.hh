#pragma once

// inner
#include <runtime/iomodel/Poller.hh>
#include <runtime/threadmodel/ThreadPool.hh>
#include <runtime/handlemodel/EventHandler.hh>


class Reactor
{
public:
    Reactor(EventHandler* event_handler);

    virtual ~Reactor();

    void Run();

    Poller& GetPoller() { return _poller; }

private:
    ThreadPool _thread_pool;
    EventHandler* _event_handler;
    Poller _poller;
};
