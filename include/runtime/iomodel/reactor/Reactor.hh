#pragma once

// inner
#include <runtime/iomodel/reactor/Poller.hh>
#include <runtime/threadmodel/ThreadPool.hh>
#include <runtime/handlemodel/EventHandler.hh>


class Reactor
{
public:
    typedef EventHandler* (*EventHandlerFactoryFunction)();

    Reactor(Poller* InPoller, EventHandler* InEventHandler);

    virtual ~Reactor();

    void Run();

private:
    ThreadPool _Threadpool;
    Poller* _Poller;
    EventHandler* MainHandler;
};
