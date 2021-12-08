#pragma once

class Poller;
class EventHandler;

class Reactor
{
public:
    struct Options 
    {
        int id;
        Poller* poller;
        EventHandler* handler;
    };

    Reactor(const Options& options);

    virtual ~Reactor();

    void Run();

private:
    Poller* _Poller;
    EventHandler* Handler;
};
