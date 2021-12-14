#pragma once

#include <runtime/handlemodel/EventHandler.hh>
#include <common/RpcTypes.hh>


class ClientConnectionManager: public EventHandler
{
public:
    ClientConnectionManager();
    virtual ~ClientConnectionManager();

    /**
     * If one EPOLLHUP triggered, we should close connection with server
     */
    virtual void HandleCloseEvent(int Fd) override;
    /**
     * Connect wrapper
     */
    int Connect(const char* ip, int port);
    /**
     * Send wrapper
     */
    void Send(const RpcMessage& Message);
private:
    int Connfd;
};
