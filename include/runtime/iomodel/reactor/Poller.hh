#pragma once

#include <functional>
#include <sys/types.h>

#include "runtime/iomodel/EventHandler.hh"

/**
 * epoll multiplexing event trigger
 */
class Poller
{
public:
    Poller();
    ~Poller();
    void Dispatch(int Timeout, EventHandler &Handler);
    void AddEvent(int Fd, uint32_t InEvents);
    void DelEvent(int Fd, uint32_t InEvents);
    void ModEvent(int Fd, uint32_t InEvents);

private:
    void CtrlEvent(int Fd, uint32_t InEvents, int Operation);
    // epoll fd
    int Epollfd;
    // max num of connections
    int MaxEvents;
    // interest set
    struct epoll_event* Events;
};
