#pragma once

#include <queue>
#include <string>
#include <functional>

#include <runtime/handlemodel/EventHandler.hh>
#include <common/RpcTypes.hh>


class ClientTransport: public EventHandler
{
    friend class RpcClient;
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
     * Send wrapper
     */
    void Send(const RpcMessage& Message);
private:
    int Connfd;
    std::queue<RpcMessage> PendingRequests;
    std::function<void()> OnNoRequestToSend;
};
