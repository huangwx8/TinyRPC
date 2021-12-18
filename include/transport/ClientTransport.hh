#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>

#include <runtime/handlemodel/EventHandler.hh>
#include <common/RpcTypes.hh>


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
     * Connect wrapper
     */
    int Connect(const char* ip, int port);
    /**
     * Send wrapper
     */
    void Send(const RpcMessage& Message);
private:
    int Connfd;

    std::mutex m;
    std::mutex m2;
    std::atomic<bool> connected;
    std::condition_variable c;
};
