#pragma once

#include <mutex>
#include <queue>
#include <string>
#include <functional>

#include <common/RpcTypes.hh>

#include <runtime/handlemodel/EventHandler.hh>
#include <runtime/iomodel/Poller.hh>


class ClientTransport: public EventHandler
{
public:
    ClientTransport();
    virtual ~ClientTransport();

    /**
     * If one EPOLLHUP triggered, we should close connection with server
     */
    virtual void HandleCloseEvent(int Fd) override;
    /**
     * If one EPOLLOUT triggered, we should select a queued rpc request and send
     */
    virtual void HandleWriteEvent(int Fd) override;
    /**
     * Connect wrapper
     */
    int Connect(std::string ip, int port);
    /**
     * Queue a new rpc request
     */
    void Push(const RpcMessage& Message);

    Poller& GetPoller() { return _poller; }

private:
    /**
     * Send wrapper
     */
    void Send(const RpcMessage& Message);
    
    // worker
    Poller _poller;
    mutable std::mutex mu{};
    // connection
    int Connfd;
    // queue
    std::queue<RpcMessage> PendingRequests;
};
