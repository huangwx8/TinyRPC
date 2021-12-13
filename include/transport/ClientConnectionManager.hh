#pragma once

#include <runtime/handlemodel/EventHandler.hh>
#include <common/RpcTypes.hh>

class Poller;
class EventHandlerManager;

class ClientConnectionManager: public EventHandler
{
public:
    ClientConnectionManager(
        EventHandlerManager* InEventHandlerMgr
        );
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
    EventHandlerManager* EventHandlerMgr;
    int Connfd;
};
