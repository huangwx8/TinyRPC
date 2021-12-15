#pragma once

// inner
#include <runtime/handlemodel/EventHandler.hh>

class RpcResultSender: public EventHandler
{
public:
    RpcResultSender() = default;
    virtual ~RpcResultSender() = default;
    /**
     * If one EPOLLOUT triggered at a connfd, try prepare a response datagram and send to it
     */
    virtual void HandleWriteEvent(int Fd) override;
};
